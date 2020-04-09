#ifndef SYSLIB_H
#define SYSLYB_H

void pchar(char c, char msg);
void pstring(char c, char msg[]);
char getchar();
void pnum(char c, int num);
int read_h();
int read_m();
int read_s();
void reboot();
void reset_tick();
int get_tick();
#endif
