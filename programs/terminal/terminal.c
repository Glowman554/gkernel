#include <stdint.h>
#include "syslib.h"


void _start(void)
{
	
	char s1[16] = "Prees h for help";
	char s2[2] = ">>";
	char s3[11] = "r -> reboot";
	char s4[9] = "h -> help";
	char s5[10] = "a -> about";
	char s6[26] = "Programiert von glowman434";
	char s7[49] = "Vielen dank an das tolle tutorial von lowlevel.eu";
	char s8[9] = "t -> time";
	char s9[9] = "e -> edit";
	char s10[16] = "Loading terminal";
	
	char buff;
	char last;
	pchar(0xf,10);
	pstring(0xa, 16, s10);
	pchar(0xf,10); // return
    pstring(0xf,16, s1); // print init text
	pchar(0xf,10);  // return
    pstring(0xf,2, s2); // print >>
    
    
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
        			pstring(0xf,11, s3); // print help text 1
					pchar(0xf,10); // return
        			pstring(0xf,9, s4);// print help text 2	
					pchar(0xf,10); // return
    				pstring(0xf,10, s5); // print help text 3
					pchar(0xf,10); // return
    				pstring(0xf,9, s8); // print help text 4
    				pchar(0xf,10); // return
    				pstring(0xf,9, s9); // print help text 5
					pchar(0xf,10); // return
					break;
				case 'a':
    				pstring(0xf,26, s6); // print about text 1
					pchar(0xf,10); // return
    				pstring(0xf,49, s7); // print about text 2
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
						for(int i = 0; i < 19000000; i++); // interval
						pchar(0x7,getchar());
					}
					break;
			}
			last = buff;
			pstring(0xf,2, s2); // print >>
		}
	}

    while(1);
}

