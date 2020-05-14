#include <stdint.h>
#include <stdbool.h>
#include "syslib.h"


void _start(void)
{	
	pchar(0xf,10);
	pstring(0xa, "Loading init");
	pchar(0xf,10); // return
	exec("terminal.bin");

    while(1);
}

