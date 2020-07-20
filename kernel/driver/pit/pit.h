#ifndef PIT_H
#define PIT_H
void init_pit(uint32_t frequency);
void pit_irq_handler();
void reset_tick();
uint32_t get_tick();
#endif
