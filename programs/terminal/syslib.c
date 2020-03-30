#include "syslib.h"
#include <stdint.h>

void pchar(char msg)
{
	asm("int $0x30" : : "a" (0), "b" (msg)); 
}

void pnum(int num)
{
	asm("int $0x30" : : "a" (3), "b" (num));
}

void pstring(int len, char msg[])
{
	for (int i = 0; i < len; i++) {
        asm("int $0x30" : : "a" (0), "b" (msg[i]));
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
