#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void init_vga();
void setpixel(int x, int y, unsigned char color);
volatile void draw_char(char c, int x, int y, uint32_t fgcolor, uint32_t bgcolor);
char getpixel(int x, int y);
#endif
