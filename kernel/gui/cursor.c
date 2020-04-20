#include "cursor.h"
#include <stdbool.h>
#include "../driver/vga/vga.h"

char pixbuf[15];

bool reloadp = false;

int x,y = 0;

void cursor_move_up() {
	//kprintf(0x4,"%d,%d\n", x,y);
	if(reloadp) loadpixel();
	y--;
	savepixel();
	reloadp = true;
	redraw();
}

void cursor_move_down() {
	//kprintf(0x4,"%d,%d\n", x,y);
	if(reloadp) loadpixel();
	y++;
	savepixel();
	reloadp = true;
	redraw();
}

void cursor_move_left() {
	//kprintf(0x4,"%d,%d\n", x,y);
	if(reloadp) loadpixel();
	x--;
	savepixel();
	reloadp = true;
	redraw();
}

void cursor_move_right() {
	//kprintf(0x4,"%d,%d\n", x,y);
	if(reloadp) loadpixel();
	x++;
	savepixel();
	reloadp = true;
	redraw();
}

void redraw() {
	setpixel(x,y,15);
	setpixel(x+1,y,15);
	setpixel(x,y+1,15);
	setpixel(x+1,y+1,15);
	setpixel(x+2,y,15);
	setpixel(x,y+2,15);
	setpixel(x+2,y+2,15);
	setpixel(x+1,y+2,15);
	setpixel(x+2,y+1,15);
	setpixel(x+3,y,15);
	setpixel(x,y+3,15);
	setpixel(x+4,y+4,15);
	setpixel(x+5,y+5,15);
	setpixel(x+3,y+3,15);
	setpixel(x+6,y+6,15);

}

void loadpixel(){
	setpixel(x,y,pixbuf[0]);
	setpixel(x+1,y,pixbuf[1]);
	setpixel(x,y+1,pixbuf[2]);
	setpixel(x+1,y+1,pixbuf[3]);
	setpixel(x+2,y,pixbuf[4]);
	setpixel(x,y+2,pixbuf[5]);
	setpixel(x+2,y+2,pixbuf[6]);
	setpixel(x+1,y+2,pixbuf[7]);
	setpixel(x+2,y+1,pixbuf[8]);
	setpixel(x+3,y,pixbuf[9]);
	setpixel(x,y+3,pixbuf[10]);
	setpixel(x+4,y+4,pixbuf[11]);
	setpixel(x+5,y+5,pixbuf[12]);
	setpixel(x+3,y+3,pixbuf[13]);
	setpixel(x+6,y+6,pixbuf[14]);

}

void savepixel() {
	pixbuf[0] = getpixel(x,y);
	pixbuf[1] = getpixel(x+1,y);
	pixbuf[2] = getpixel(x,y+1);
	pixbuf[3] = getpixel(x+1,y+1);
	pixbuf[4] = getpixel(x+2,y);
	pixbuf[5] = getpixel(x,y+2);
	pixbuf[6] = getpixel(x+2,y+2);
	pixbuf[7] = getpixel(x+1,y+2);
	pixbuf[8] = getpixel(x+2,y+1);
	pixbuf[9] = getpixel(x+3,y);
	pixbuf[10] = getpixel(x,y+3);
	pixbuf[11] = getpixel(x+4,y+4);
	pixbuf[12] = getpixel(x+5,y+5);
	pixbuf[13] = getpixel(x+3,y+3);
	pixbuf[14] = getpixel(x+6,y+6);

}

int getcx()
{
	return x;
}

int getcy()
{
	return y;
}
