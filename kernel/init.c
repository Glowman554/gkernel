#include "console.h"
#include "intr.h"
#include "mm.h"
#include "multiboot.h"
#include "driver/keyboard/keyboard.h"
#include "driver/cmos/cmos.h"
#include "driver/pit/pit.h"
#include "driver/vga/vga.h"
#include "widget.h"
#include "fs.h"
#include "initrd.h"
#include "config.h"
#include "bios.h"
#include "pci.h"
#include "string.h"
#include "serial.h"

void task_a(){
	register uint32_t input asm("ebx");
	asm("int $0x30" : : "a" (KVEN));
	kprintf(0xf, "%s\n", input);
	asm("int $0x30" : : "a" (EXIT), "b" (0));
	while(1);
}

void task_b(){
	while(1) kprintf(0xf, "b\n");
}

void task_c(){
	while(1) kprintf(0xf, "c\n");
}

void task_d(){
	while(1) kprintf(0xf, "d\n");
}


void init(struct multiboot_info *mb_info)
{
	init_serial();
    clrscr();
    kprintf(0xa, "GKernel %d Loading...\n", VERSION);
	kprintf(0xf, "Reporting kernel version %d\n", VERSION);
	kprintf(0xf, "Reporting kernel vendor %s\n", VENDOR);
    kprintf(0xf, "Its %d:%d:%d\n\n", read_h(), read_m(), read_s());
	kprintf(0xf, "Init physical memory\n");
    pmm_init(mb_info);	
	kprintf(0xf, "Init global descriptor tabel\n");
    init_gdt();
    kprintf(0xf, "Init interupts\n");
    init_intr();
	kprintf(0xf, "Calling init_keyboard()\n");
    init_keyboard();
	kprintf(0xf, "Calling init_pit()\n");
    init_pit(5);
	//asm volatile("int $0x1");
	kprintf(0xf, "Init multitasking\n");
	init_multitasking(mb_info);
	//init_task(task_a);
	//init_task(task_b);
	//init_task(task_c);
	//init_task(task_d);
}	
