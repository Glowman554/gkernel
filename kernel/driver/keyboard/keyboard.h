#ifndef KEYBOARD_H
#define KEYBOARD_H

void init_keyboard(void);
char getchar();
void keyboard_irq_handler(uint8_t irq);
unsigned char inb(unsigned short _port);
void outb(unsigned short _port, unsigned char _data);


#endif
