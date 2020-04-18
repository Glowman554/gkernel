#include <stddef.h>
#include <stdbool.h>
#include "string.h"

#include "console.h"
#include "intr.h"
#include "mm.h"
#include "multiboot.h"
#include "elf.h"
#include "driver/keyboard/keyboard.h"
#include "widget.h"
#include "fs.h"
#include "initrd.h"

struct task {
    struct cpu_state*   cpu_state;
    struct task*        next;
};

static struct task* first_task = NULL;
static struct task* current_task = NULL;

void reboot();

static void terminal(void)
{
	kprintf(0xf,"\nStarting miniterm\n");
	kprintf(0xf,"Press h for help\n");
	kprintf(0xf,">>");
	char in;
	while(1){
		in = getchar();
		kprintf(0xf,"%c\n", in);
		
		switch(in){
			case 'h':
				kprintf(0xf,"h -> help\n");
				kprintf(0xf,"e -> edit\n");
				kprintf(0xf,"r -> reboot\n");
				kprintf(0xf,"w -> Debug Error\n");
				kprintf(0xf,"v -> init vga\n");
				break;
			case 'e':
				while(1) kprintf(0xf,"%c", getchar());
				break;
			case 'r':
				reboot();
				break;
			case 'w':
				asm("int $0x30" : : "a" (9));
				break;
			case 'v':
				asm("int $0x30" : : "a" (13));
				break;
		}
		kprintf(0xf,">>");
	}
}


void reboot()
{
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    asm volatile ("hlt");
}

/*
 * Jeder Task braucht seinen eigenen Stack, auf dem er beliebig arbeiten kann,
 * ohne dass ihm andere Tasks Dinge ueberschreiben. Ausserdem braucht ein Task
 * einen Einsprungspunkt.
 */
struct task* init_task(void* entry)
{
	//kprintf(0xa, "Calling pmm_alloc\n");
    uint8_t* stack = pmm_alloc();
    uint8_t* user_stack = pmm_alloc();

    /*
     * CPU-Zustand fuer den neuen Task festlegen
     */
    struct cpu_state new_state = {
        .eax = 0,
        .ebx = 0,
        .ecx = 0,
        .edx = 0,
        .esi = 0,
        .edi = 0,
        .ebp = 0,
        .esp = (uint32_t) user_stack + 4096,
        .eip = (uint32_t) entry,

        /* Ring-3-Segmentregister */
        .cs  = 0x18 | 0x03,
        .ss  = 0x20 | 0x03,

        /* IRQs einschalten (IF = 1) */
        .eflags = 0x200,
    };

    /*
     * Den angelegten CPU-Zustand auf den Stack des Tasks kopieren, damit es am
     * Ende so aussieht als waere der Task durch einen Interrupt unterbrochen
     * worden. So kann man dem Interrupthandler den neuen Task unterschieben
     * und er stellt einfach den neuen Prozessorzustand "wieder her".
     */
    //kprintf(0xa, "Init cpu_state\n");
    struct cpu_state* state = (void*) (stack + 4096 - sizeof(new_state));
    *state = new_state;

    /*
     * Neue Taskstruktur anlegen und in die Liste einhaengen
     */
    //kprintf(0xa, "Calling pmm_alloc\n");
    struct task* task = pmm_alloc();
    task->cpu_state = state;
    task->next = first_task;
    first_task = task;
    return task;
}

void init_elf(void* image)
{
    /*
     * FIXME Wir muessen eigentlich die Laenge vom Image pruefen, damit wir bei
     * korrupten ELF-Dateien nicht ueber das Dateiende hinauslesen.
     */

    struct elf_header* header = image;
    struct elf_program_header* ph;
    int i;

    /* Ist es ueberhaupt eine ELF-Datei? */
    if (header->magic != ELF_MAGIC) {
        kprintf(0x4, "Keine gueltige ELF-Magic!\n");
        return;
    }

    /*
     * Alle Program Header durchgehen und den Speicher an die passende Stelle
     * kopieren.
     *
     * FIXME Wir erlauben der ELF-Datei hier, jeden beliebigen Speicher zu
     * ueberschreiben, einschliesslich dem Kernel selbst.
     */
    ph = (struct elf_program_header*) (((char*) image) + header->ph_offset);
    for (i = 0; i < header->ph_entry_count; i++, ph++) {
        void* dest = (void*) ph->virt_addr;
        void* src = ((char*) image) + ph->offset;

        /* Nur Program Header vom Typ LOAD laden */
        if (ph->type != 1) {
            continue;
        }

        memset(dest, 0, ph->mem_size);
        memcpy(dest, src, ph->file_size);
    }

    init_task((void*) header->entry);
}

bool test_elf_header(void* image);

void init_multitasking(struct multiboot_info* mb_info)
{
	if (mb_info->mbs_mods_count == 0){
		kprintf(0x4,"No multiboot modules\n");
		init_task(terminal);
	}
	
	
	if (mb_info->mbs_mods_count != 0) {
     	struct multiboot_module* modules = mb_info->mbs_mods_addr;
       	int i;

       	for (i = 0; i < mb_info->mbs_mods_count; i++) {
       		if(test_elf_header((void*) modules[i].mod_start)){
			
           		init_elf((void*) modules[i].mod_start);
           	}else if(mb_info->mbs_mods_count == 1){
           		initialise_initrd((void*) modules[i].mod_start);
           		init_task(terminal);
			}else{
				initialise_initrd((void*) modules[i].mod_start);
			}
       	}
   	}
}

bool test_elf_header(void* image)
{

    struct elf_header* header = image;

    /* Ist es ueberhaupt eine ELF-Datei? */
    if (header->magic != ELF_MAGIC) {
        
        return false;
    } else {
    	return true;
	}

}

/*
 * Gibt den Prozessorzustand des naechsten Tasks zurueck. Der aktuelle
 * Prozessorzustand wird als Parameter uebergeben und gespeichert, damit er
 * beim naechsten Aufruf des Tasks wiederhergestellt werden kann
 */
struct cpu_state* schedule(struct cpu_state* cpu)
{
    /*
     * Wenn schon ein Task laeuft, Zustand sichern. Wenn nicht, springen wir
     * gerade zum ersten Mal in einen Task. Diesen Prozessorzustand brauchen
     * wir spaeter nicht wieder.
     */
    //kprintf("Save task state\n");
    if (current_task != NULL) {
        current_task->cpu_state = cpu;
    }

    /*
     * Naechsten Task auswaehlen. Wenn alle durch sind, geht es von vorne los
     */
    //kprintf("Load next task\n");
    if (current_task == NULL) {
        current_task = first_task;
    } else {
        current_task = current_task->next;
        if (current_task == NULL) {
            current_task = first_task;
        }
    }
	//kprintf("Activate cpu state\n");
    /* Prozessorzustand des neuen Tasks aktivieren */
    cpu = current_task->cpu_state;

    return cpu;
}
