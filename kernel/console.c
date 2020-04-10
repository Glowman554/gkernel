#include <stdarg.h>
#include <stdbool.h>
#include "console.h"

bool scrole = true;

static int x = 0;
static int y = 0;

static char* video = (char*) 0xb8000;

char color = 0xf;

static int kprintf_res = 0;

static inline void outb(unsigned short port, unsigned char data)
{
    asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}
void setscrole(bool s)
{
	scrole = s;
}
static void kputc(char c)
{
    if ((c == '\n') || (x > 79)) {
        x = 0;
        y++;
    }

    // Die folgende Zeile reinnehmen fuer Ausgabe auf der seriellen
    // Schnittstelle (nur Emulatoren; reale Hardware braucht mehr)
    // outb(0x3f8, c);

    if (c == '\n') {
        return;
    }

    if (scrole && y > 22) {
        int i;
        for (i = 0; i < 2 * 22 * 80; i++) {
            video[i] = video[i + 160];
        }

        for (; i < 2 * 25 * 80; i++) {
            video[i] = 0;
        }
        y--;
    }

    video[2 * (y * 80 + x)] = c;
    video[2 * (y * 80 + x) + 1] = color;

    x++;
    kprintf_res++;
}

static void kputs(const char* s)
{
    while (*s) {
        kputc(*s++);
    }
}

void kputn(unsigned long x, int base)
{
    char buf[65];
    const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* p;

    if (base > 36) {
        return;
    }

    p = buf + 64;
    *p = '\0';
    do {
        *--p = digits[x % base];
        x /= base;
    } while (x);
    kputs(p);
}

void clrscr(void)
{
    int i;
    for (i = 0; i < 2 * 25 * 80; i++) {
        video[i] = 0;
    }

    x = y = 0;
}

void setx(int i){
	x = i;
}

void sety(int i){
	y = i;
}

int getx(){
	return x;
}

int gety(){
	return y;
}

void setcolor(char c){
	color = c;
}

int kprintf(char c, const char* fmt, ...)
{
    va_list ap;
    const char* s;
    unsigned long n;
	color = c;
    va_start(ap, fmt);
    kprintf_res = 0;
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's':
                    s = va_arg(ap, char*);
                    kputs(s);
                    break;
                case 'c':
                    n = va_arg(ap, int);
                    kputc(n);
                    break;
                case 'd':
                case 'u':
                    n = va_arg(ap, unsigned long int);
                    kputn(n, 10);
                    break;
                case 'x':
                case 'p':
                    n = va_arg(ap, unsigned long int);
                    kputn(n, 16);
                    break;
                case '%':
                    kputc('%');
                    break;
                case '\0':
                    goto out;
                default:
                    kputc('%');
                    kputc(*fmt);
                    break;
            }
        } else {
            kputc(*fmt);
        }

        fmt++;
    }

out:
    va_end(ap);

    return kprintf_res;
}
