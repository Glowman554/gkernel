#include "vga.h"
#include "bios.h"
#include "fonts.h"
#include <stdint.h>

#define NULL ((void*) 0)

extern void int32(unsigned char intnum, regs16_t *regs);
unsigned char* VGA = (unsigned char*) 0xA0000;


void init_vga(){
	regs16_t regs;
	//            x   y  color
    // switch to 320x200x256 graphics mode
    regs.ax = 0x0013;
    int32(0x10, &regs);
}

void setpixel(int x, int y, unsigned char color) {
  	int offset;
  	if(0 <= x && x < 320) {
    	if(0 <= y && y < 200) {
      		offset = 320*y + x;
      		VGA[offset] = color;
    	}
  	}
}

char getpixel(int x, int y) {
  	int offset;
  	if(0 <= x && x < 320) {
    	if(0 <= y && y < 200) {
      		offset = 320*y + x;
      		return VGA[offset];
    	}
  	}
	
	return 0;
	
}

volatile void draw_char(char c, int x, int y, uint32_t fgcolor, uint32_t bgcolor) {
	int cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128};
	const char *glyph=LSB[((int)c)];
		
	for(cy=0;cy<14;cy++){
		for(cx=0;cx<8;cx++){
			setpixel(cx+x,cy+y,glyph[cy]&mask[cx]?fgcolor:bgcolor);
		}
	}
}
