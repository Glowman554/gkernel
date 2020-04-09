#include <stdint.h>
#include "syslib.h"


void _start(void)
{	
	char buff;
	char last;
	pchar(0xf,10);
	pstring(0xa, "Loading terminal");
	pchar(0xf,10); // return
    pstring(0xf, "Press h for help");
	pchar(0xf,10);  // return
    pstring(0xf, ">>");
    
    
    while(1){
    	buff = getchar();
    	if(buff != last){
    		pchar(0x7,buff); // print buff
    		pchar(0xf,10); // return
    		
    		switch(buff){
    			case 'r':
	        		reboot();
					break;
	        	case 'h':
        			pstring(0xf, "r -> reboot");
					pchar(0xf,10); // return
        			pstring(0xf, "h -> help");
					pchar(0xf,10); // return
    				pstring(0xf, "a -> about");
					pchar(0xf,10); // return
    				pstring(0xf, "t -> time");
    				pchar(0xf,10); // return
    				pstring(0xf, "e -> edit");
					pchar(0xf,10); // return
					break;
				case 'a':
    				pstring(0xf, "Programiert von glowman434");
					pchar(0xf,10); // return
    				pstring(0xf, "Vielen dank an das tolle tutorial von lowlevel.eu");
					pchar(0xf,10); // return
					break;
				case 't':
					pnum(0xf,read_h());
					pchar(0xf,':');
					pnum(0xf,read_m());
					pchar(0xf,':');
					pnum(0xf,read_s());
					pchar(0xf,10);
					break;
				case 'e':
					while(1){
						reset_tick();
						while(get_tick() < 5);
						pchar(0x7,getchar());
					}
					break;
			}
			last = buff;
			pstring(0xf, ">>");
		}
	}

    while(1);
}

