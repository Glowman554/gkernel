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
#include "tasks.h"

static struct task* first_task = NULL;
static struct task* current_task = NULL;



struct task* init_task(void* entry);

int nextpid = 1;
int proccount = 0;

void kexit(int code){
	kprintf(0xf, "[%d] Exit with error code %d\n", current_task->pid, code);
	proccount--;
	struct task* temp = first_task;
	if(current_task->pid == first_task->pid){ 
		first_task = temp->next;
		return;
	}
	while(temp->next != NULL){
		if(temp->next->pid == current_task->pid){
			temp->next = temp->next->next;
			current_task = temp->next;
		}
		temp = temp->next;
	}
}

/*
 * Jeder Task braucht seinen eigenen Stack, auf dem er beliebig arbeiten kann,
 * ohne dass ihm andere Tasks Dinge ueberschreiben. Ausserdem braucht ein Task
 * einen Einsprungspunkt.
 */
struct task* init_task(void* entry) {
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
	task->pid = nextpid;
	nextpid++;
	kprintf(0xf, "Starting Task with pid %d\n", task->pid);
	proccount++;
	
	first_task = task;
	return task;
}

void init_elf(void* image) {
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

void init_multitasking(struct multiboot_info* mb_info) {
	
	
	if (mb_info->mbs_mods_count == 0) {
		kprintf(0x4,"No multiboot modules\n");
		kprintf(0xf, "Cant load init.bin without initrd\n");
	}
	//init_task(cursor_manager);

	if (mb_info->mbs_mods_count != 0) {
		struct multiboot_module* modules = mb_info->mbs_mods_addr;
		
		kprintf(0xf, "Loading %s as initrd\n", modules[0].cmdline);
		fs_root = initialise_initrd((void*) modules[0].mod_start);
		/*
		int i = 0;
		struct dirent *node = 0;
	
		kprintf(0x8, "\n");
	
		while ( (node = readdir_fs(fs_root, i)) != 0)
		{
			int len = strlen(node->name);
			fs_node_t *fsnode = finddir_fs(fs_root, node->name);
			
			if ((fsnode->flags&0x7) == FS_DIRECTORY)
				kprintf(0x8,"Found Directory ");
			else
				kprintf(0x8,"Found File ");
		
			for(int i = 0; i < len; i++) kprintf(0x8, "%c", node->name[i]);
			kprintf(0x8, "\n");
			i++;
		}
		*/
		kprintf(0xf, "Loading init.bin from initrd\n");
		fs_node_t *fsnode = finddir_fs(fs_root, "init.bin");
		
		char buf[10000];
		
		read_fs(fsnode, 0, 10000, buf);
		
		//kprintf(0xa, "%c%c%c%c\n", buf[0], buf[1], buf[2], buf[3]);
		init_elf((void*) buf);
	}


}

bool test_elf_header(void* image) {

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
struct cpu_state* schedule(struct cpu_state* cpu) {
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

int getproccount(){
	return proccount;
}
