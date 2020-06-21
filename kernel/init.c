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
#include "gui/desktop.h"
#include "gui/info_app.h"
#include "pci.h"
#include "string.h"

void init(struct multiboot_info *mb_info)
{
    clrscr();
    kprintf(0xa, "GKernel %d Loading...\n", VERSION);
	kprintf(0xf, "Reporting kernel version %d\n", VERSION);
    kprintf(0xf, "Its %d:%d:%d\n\n", read_h(), read_m(), read_s());
	kprintf(0xf, "Init physical memory\n");
    pmm_init(mb_info);	
	kprintf(0xf, "Init global descriptor tabel\n");
    init_gdt();
    kprintf(0xf, "Init interupts\n");
    init_intr();
    
    //asm volatile("int $0x1");
	kprintf(0xf, "Calling init_keyboard()\n");
    init_keyboard();
	kprintf(0xf, "Calling init_pit()\n");
    init_pit(5);
	kprintf(0xf, "Init multitasking\n");
	init_multitasking(mb_info);
}	
