#include "info_app.h"
#include "desktop.h"
#include "string.h"
#include "../driver/vga/vga.h"

void init_info_app()
{
	for(int x = 0; x < 20; x++){
		for(int y = 0; y < 20; y++){
			setpixel(x+5,y+20,8);
		}
	}
	
	setpixel(13,20,15);
	setpixel(14,20,15);
	setpixel(15,20,15);
	setpixel(16,20,15);
	setpixel(13,21,15);
	setpixel(14,21,15);
	setpixel(15,21,15);
	setpixel(16,21,15);
	setpixel(13,22,15);
	setpixel(14,22,15);
	setpixel(15,22,15);
	setpixel(16,22,15);
	setpixel(13,23,15);
	setpixel(14,23,15);
	setpixel(15,23,15);
	setpixel(16,23,15);
	
	for(int y = 0; y < 10; y++){
		setpixel(13,28+y,15);
		setpixel(14,28+y,15);
		setpixel(15,28+y,15);
		setpixel(16,28+y,15);
	}
}

void info_app_main()
{
	create_view(30,35,210,100,"info");
	
	int ox = 31;
	int oy = 36;
	char i1[] = "GlowOS from Glowman554";
	char i2[] = "Tutorial from lowlevel.eu";
	
	int len = strlen(i1);
	for(int i = 0; i < len; i++)
	{
		draw_char(i1[i], ox+1+(i*8), oy, 10, 7);	
	}
	len = strlen(i2);
	for(int i = 0; i < len; i++)
	{
		draw_char(i2[i], ox+1+(i*8), oy+14, 10, 7);	
	}
}
