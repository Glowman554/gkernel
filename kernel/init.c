#include "console.h"
#include "intr.h"
#include "mm.h"
#include "multiboot.h"
#include "driver/keyboard/keyboard.h"
#include "driver/cmos/cmos.h"
#include "widget.h"

char* version = "v0.1.8";

void init(struct multiboot_info *mb_info)
{
    clrscr();
    kprintf(0xa, "GlowOS %s Loading...\n", version);
    kprintf(0xf, "Its %d:%d:%d\n\n", read_h(), read_m(), read_s());
    pmm_init(mb_info);	
    init_gdt();
    
    init_intr();
    
    //asm volatile("int $0x1");
    init_keyboard();
    init_multitasking(mb_info);
}
