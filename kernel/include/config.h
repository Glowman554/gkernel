#ifndef CONFIG_H
#define CONFIG_H
#include "stdbool.h"

#define VERSION 28

//syscalls

#define PUTC 	0
#define GETCHAR 1
#define REBOOT 	2
#define PNUM 	3
#define READ_H	4
#define READ_M 	5
#define READ_S	6
#define RTICK	7
#define GTICK	8
#define GETMOV	9
#define CLRSCR	10
#define SETX 	11
#define SETY 	12
#define SVGA	14
#define SPIXEL	15
#define LOADF	17
#define LS		18
#define LSPCI	19
#define KVER	20

#endif