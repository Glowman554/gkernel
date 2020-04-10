#ifndef CONSOLE_H
#define CONSOLE_H
#include <stdbool.h>
void clrscr(void);
void kputn(unsigned long x, int base);
void setx(int i);
void sety(int i);
int getx();
int gety();
void setcolor(char c);
void setscrole(bool s);
int kprintf(char c, const char* fmt, ...);

#endif
