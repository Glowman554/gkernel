#include <stdint.h>
#include <stdbool.h>
#include "syslib.h"


void _start(void)
{	
	pchar(0xf,10);
	pstring(0xa, "Loading init");
	pchar(0xf,10); // return
	pstring(0xf, "Kernel Reports version ");
	pnum(0xf, kversion());
	pchar(0xf, 10);
	exec("terminal.bin");
    while(1);
}

