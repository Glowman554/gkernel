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
#include "bios.h"
#include "gui/desktop.h"
#include "gui/info_app.h"

char* version = "v0.2.1";

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
    init_pit(5);
	init_multitasking(mb_info);
	//init_vga();
	//init_desktop(version);
	//init_info_app();
	
	
	//create_view(20,35,200,100,"Test");
	//create_view(200,20,100,50,"Test2");
	
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
}	
