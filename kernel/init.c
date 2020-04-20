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
	while ( (node = readdir_fs(fs_root, i)) != 0)
	{
  		kprintf(0xf,"\nFound file ");
  		int len = strlen(node->name);

  		//kprintf(0x4, "%d", len);

		char new_name[len];

  		strcpy(new_name, node->name);

  		if(node->name[len-1] == 'g' && node->name[len-2] == 'm' && node->name[len-3] == 'i' && node->name[len-4] == '.'){
  			new_name[len-1] = ' '; 
			new_name[len-2] = ' ';
			new_name[len-3] = ' ';
			new_name[len-4] = ' ';

		  }

  		for(int i = 0; i < len; i++) kprintf(0xf, "%c", new_name[i]);
  		kprintf(0xf, "\n");
  		fs_node_t *fsnode = finddir_fs(fs_root, node->name);

 		if ((fsnode->flags&0x7) == FS_DIRECTORY)
    		kprintf(0xf,"(directory)\n");
  		else
  		{
    		char buf[256];
    		uint32_t sz = read_fs(fsnode, 0, 256, buf);
    		int j;
    		for (j = 0; j < sz; j++)
      			kprintf(0x7, "%c", buf[j]);



  		}
  		i++;
	}
}	
