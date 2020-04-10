#include "syslib.h"
#include <stdint.h>

void pchar(char c, char msg)
{
	asm("int $0x30" : : "a" (0), "b" (msg), "c" (c)); 
}

void pnum(char c, int num)
{
	asm("int $0x30" : : "a" (3), "b" (num), "c" (c));
}

void pstring(char c, char msg[])
{
	int i;
    for (i = 0; msg[i] != '\0'; i++) {
    	asm("int $0x30" : : "a" (0), "b" (msg[i]), "c" (c));
    }
}

char getchar()
{
	register char input asm("ebx");
	asm("int $0x30" : : "a" (1));
	return input;
}

int read_h()
{
	register uint32_t input asm("ebx");
	asm("int $0x30" : : "a" (4));
	return input;
}

int read_m()
{
	register int input asm("ebx");
	asm("int $0x30" : : "a" (5));
	return input;
}

int read_s()
{
	register int input asm("ebx");
	asm("int $0x30" : : "a" (6));
	return input;
}

void reboot()
{
	asm("int $0x30" : : "a" (2));
}

void reset_tick()
{
	asm("int $0x30" : : "a" (7));
}

int get_tick()
{
	register int input asm("ebx");
	asm("int $0x30" : : "a" (8));
	return input;
}

void setx(int x)
{
	asm("int $0x30" : : "a" (11), "b" (x));
}
void sety(int y)
{
	asm("int $0x30" : : "a" (12), "b" (y));
}

void clrscr()
{
	asm("int $0x30" : : "a" (10));
}
