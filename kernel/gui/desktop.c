#include "desktop.h"
#include "string.h"
#include "../driver/vga/vga.h"


void init_desktop()
{
	for(int x = 0; x < 320; x++){
		for(int y = 0; y < 200; y++){
			setpixel(x, y, 7);
		}
	}	
	for(int i = 0; i < 320; i++){
		setpixel(i, 15, 8);
		setpixel(i, 14, 8);
		setpixel(i, 13, 8);
		setpixel(i, 12, 8);
		setpixel(i, 11, 8);
		setpixel(i, 10, 8);
		setpixel(i, 9, 8);
		setpixel(i, 8, 8);
		setpixel(i, 7, 8);
		setpixel(i, 6, 8);
		setpixel(i, 5, 8);
		setpixel(i, 4, 8);
		setpixel(i, 3, 8);
		setpixel(i, 2, 8);
		setpixel(i, 1, 8);
		setpixel(i, 0, 8);
	}
	
	int x;
	char s[] = "GlowOS";
	int len = strlen(s);
	for(int i = 0; i < len; i++)
	{
		draw_char(s[i], 1+(i*8), 1, 15, 8);	
		x = 1+(i*8);
	}
}

void create_view(int ox, int oy, int sizex, int sizey, char name[])
{
	for(int i = 0; i < sizex+2; i++){
		setpixel(ox+i, oy-15, 0);
		setpixel(ox+i, oy-14, 0);
		setpixel(ox+i, oy-13, 0);
		setpixel(ox+i, oy-12, 0);
		setpixel(ox+i, oy-11, 0);
		setpixel(ox+i, oy-10, 0);
		setpixel(ox+i, oy-9, 0);
		setpixel(ox+i, oy-8, 0);
		setpixel(ox+i, oy-7, 0);
		setpixel(ox+i, oy-6, 0);
		setpixel(ox+i, oy-5, 0);
		setpixel(ox+i, oy-4, 0);
		setpixel(ox+i, oy-3, 0);
		setpixel(ox+i, oy-2, 0);
		setpixel(ox+i, oy-1, 0);
		setpixel(ox+i, oy, 0);
		setpixel(ox+i, oy+sizey+1, 0);
		
	}
	
	
	int len = strlen(name);
	for(int i = 0; i < len; i++)
	{
		draw_char(name[i], ox+1+(i*8), oy-14, 10, 0);	
	}
	
	for(int i = 0; i < sizey+2; i++){
	
		setpixel(ox, oy+i, 0);
		setpixel(ox+sizex+1, oy+i, 0);
		
	}
}

