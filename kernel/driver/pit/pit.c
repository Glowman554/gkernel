#include <stdint.h>
#include "pit.h"
#include "intr.h"
#include "io.h"
#include "info_line.h"
#include "console.h"
#include "tasks.h"

uint32_t tick = 0;

void init_pit(uint32_t frequency)
{
	int counter = 1193182 / frequency;
   	outb(0x43, 0x34);
   	outb(0x40,counter & 0xFF);
   	outb(0x40,counter >> 8);
   	register_interrupt_handler(32, &pit_irq_handler);
}

void pit_irq_handler()
{
	tick++;
	//kprintf(0xa, "Tick %d\n", tick);
	drawinfo(getproccount());
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
