#include <stddef.h>
#include <string.h>

#include "console.h"
#include "intr.h"
#include "mm.h"
#include "multiboot.h"
#include "elf.h"
#include "driver/keyboard/keyboard.h"

struct task {
    struct cpu_state*   cpu_state;
    struct task*        next;
};

static struct task* first_task = NULL;
static struct task* current_task = NULL;

void reboot();

static void terminal(void)
{
	char buff;
	char last;
	register uint32_t input asm("ebx");
	kprintf("\nPress h for help\n");
	kprintf(">> ");
    while (1) {
    	asm("int $0x30" : : "a" (1));
        buff = input;
        if(buff != last){
        	kprintf("%c\n", buff);
	        switch(buff){
	        	case 'r':
	        		reboot();
	        		break;
	        	case 'h':
	        		kprintf("r -> reboot\n");
	        		kprintf("h -> help\n");
	        		break;
			}
			last = buff;
			kprintf(">> ");
		}
		//asm("int $0x30" : : "a" (1));
		
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
	//kprintf("Calling pmm_alloc\n");
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
    //kprintf("Init cpu_state\n");
    struct cpu_state* state = (void*) (stack + 4096 - sizeof(new_state));
    *state = new_state;

    /*
     * Neue Taskstruktur anlegen und in die Liste einhaengen
     */
    //kprintf("Calling pmm_alloc\n");
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
        kprintf("Keine gueltige ELF-Magic!\n");
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

void init_multitasking(struct multiboot_info* mb_info)
{

        //kprintf("Calling init_task\n");
        //init_task(terminal);
        //init_task(task_b);
        //init_task(task_c);
        //init_task(task_d);
        /*
         * Wenn wir mindestens ein Multiboot-Modul haben, kopieren wir das
         * erste davon nach 2 MB und erstellen dann einen neuen Task dafuer.
         */
        //kprintf("Geting multiboot modules\n");
        if (mb_info->mbs_mods_count != 0) {
        	struct multiboot_module* modules = mb_info->mbs_mods_addr;
        	int i;

        	for (i = 0; i < mb_info->mbs_mods_count; i++) {
        		//kprintf("Calling init_elf\n");
            	init_elf((void*) modules[i].mod_start);
        	}
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
