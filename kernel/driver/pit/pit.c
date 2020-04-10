#include <stdint.h>
#include "pit.h"
#include "io.h"
#include "info_line.h"
#include "console.h"

uint32_t tick = 0;

void init_pit(uint32_t frequency)
{
	kprintf(0xf, "Init pit  ");
	int counter = 1193182 / frequency;
   	outb(0x43, 0x34);
   	outb(0x40,counter & 0xFF);
   	outb(0x40,counter >> 8);
   	kprintf(0xf,"[");
	kprintf(0x2,"OK");
    kprintf(0xf,"]\n");
}

void pit_irq_handler(uint8_t irq)
{
	tick++;
	//kprintf(0xa, "Tick %d\n", tick);
	drawinfo(tick);
	if(tick > 1000000) tick = 0;
}

void reset_tick()
{
	tick = 0;
}

uint32_t get_tick()
{
	return tick;
}
