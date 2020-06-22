#include "vga.h"
#include "bios.h"
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
