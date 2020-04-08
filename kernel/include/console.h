#ifndef CONSOLE_H
#define CONSOLE_H

void clrscr(void);
void kputn(unsigned long x, int base);
void setx(int i);
void sety(int i);
void setcolor(char c);
int kprintf(char c, const char* fmt, ...);

#endif
