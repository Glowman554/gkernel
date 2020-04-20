#include "files.h"
#include "desktop.h"
#include "string.h"
#include "fs.h"
#include "initrd.h"
#include "../driver/vga/vga.h"

char logo[20][20] = {
	{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
	{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,7,7,7,7,7,7,},
	{7,7,1,1,1,7,7,7,7,7,7,7,1,1,1,7,7,7,7,7,},
	{7,7,1,1,1,7,7,7,7,1,1,7,1,1,1,1,7,7,7,7,},
	{7,7,1,1,1,7,7,7,7,1,1,7,1,1,1,1,1,7,7,7,},
	{7,7,1,1,1,7,7,7,7,1,1,7,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,7,7,7,7,1,1,7,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,7,7,7,7,7,7,7,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,},
	{7,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,},
	{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
	{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
	
};

void init_files_app()
{
	for(int x = 0; x < 20; x++){
		for(int y = 0; y < 20; y++){
			setpixel(x+5,y+45,logo[y][x]);
		}
	}
}

void files_app_main()
{
	asm("int $0x30" : : "a" (14));
	create_view(30,35,210,100,"files");
	
	int ox = 31;
	int oy = 36;
	int i = 0;
	struct dirent *node = 0;
	while ( (node = readdir_fs(fs_root, i)) != 0)
	{
		int len = strlen(node->name);
		char new_name[len];
		strcpy(new_name, node->name);
		if(node->name[len-1] == 'g' && node->name[len-2] == 'm' && node->name[len-3] == 'i' && node->name[len-4] == '.'){
  			new_name[len-1] = ' '; 
			new_name[len-2] = ' ';
			new_name[len-3] = ' ';
			new_name[len-4] = ' ';

		}
		for(int i = 0; i < len; i++)
		{
			draw_char(new_name[i], ox+1+(i*8), oy, 10, 7);	
		}
		oy = oy + 14;
		i++;
	}
}
