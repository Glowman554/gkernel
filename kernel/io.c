#include <stdint.h>
#include "io.h"

unsigned char inb(unsigned short _port)
{
	unsigned char result;
	__asm__ ("inb %1, %0" : "=a" (result) : "Nd" (_port));
	return result;
}

void outb(unsigned short _port, unsigned char _data)
{
	__asm__ ("outb %0, %1" : : "a" (_data), "Nd" (_port));
}
