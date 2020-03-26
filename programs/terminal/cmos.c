#include <stdint.h>
#include "cmos.h"


#define CMOS_PORT_ADDRESS 0x70
#define CMOS_PORT_DATA    0x71


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

uint8_t cmos_read(uint8_t address) {
  	//Wenn eines der Zeit/Datums Register ausgelesen werden soll, muss
    // sichergestellt werden, dass keine aktualisierung am laufen ist.
    if(address < 10)
    {
        outb(0x70, 0xa);
        while((inb(0x71) & (1 << 7)) != 0)
        {
            asm volatile("nop");
        }
    }

    outb(0x70, address);
    return inb(0x71);

}


static uint8_t convert(uint8_t num)
{
    //Ueberpruefen, ob BCD verwendet wird
    if((cmos_read(0xb) & (1 << 2)) == 0)
    {
        return (num & 0xf) + ((num >> 4) & 0xf) * 10;
    }
    else
    {
        return num;
    }
}


void print_time(void)
{
	//https://lowlevel.eu/wiki/CMOS
	int s = cmos_read(0x00);
	int m = cmos_read(0x02);
	int h = cmos_read(0x04);
	
	asm("int $0x30" :: "a" (3), "b" (convert(h)));
	asm("int $0x30" :: "a" (0), "b" (':'));
	asm("int $0x30" :: "a" (3), "b" (convert(m)));
	asm("int $0x30" :: "a" (0), "b" (':'));
	asm("int $0x30" :: "a" (3), "b" (convert(s)));
	asm("int $0x30" : : "a" (0), "b" (10));
}



