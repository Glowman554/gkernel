#include "console.h"
#include "intr.h"
#include "mm.h"
#include "multiboot.h"
#include "driver/keyboard/keyboard.h"

char* version = "v0.1.1";

void init(struct multiboot_info *mb_info)
{
    clrscr();
    kprintf("GlowOS %s Loading...\n", version);
    pmm_init(mb_info);	
    init_gdt();
    
    init_intr();
    
    //asm volatile("int $0x0");
    init_keyboard();
    init_multitasking(mb_info);
}
