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
	pchar(10);
	pstring(16, s10);
	pchar(10); // return
    pstring(16, s1); // print init text
	pchar(10);  // return
    pstring(2, s2); // print >>
    
    
    while(1){
    	buff = getchar();
    	if(buff != last){
    		pchar(buff); // print buff
    		pchar(10); // return
    		
    		switch(buff){
    			case 'r':
	        		reboot();
					break;
	        	case 'h':
        			pstring(11, s3); // print help text 1
					pchar(10); // return
        			pstring(9, s4);// print help text 2	
					pchar(10); // return
    				pstring(10, s5); // print help text 3
					pchar(10); // return
    				pstring(9, s8); // print help text 4
    				pchar(10); // return
    				pstring(9, s9); // print help text 5
					pchar(10); // return
					break;
				case 'a':
    				pstring(26, s6); // print about text 1
					pchar(10); // return
    				pstring(49, s7); // print about text 2
					pchar(10); // return
					break;
				case 't':
					pnum(read_h());
					pchar(':');
					pnum(read_m());
					pchar(':');
					pnum(read_s());
					pchar(10);
					break;
				case 'e':
					while(1){
						for(int i = 0; i < 19000000; i++); // interval
						pchar(getchar());
					}
					break;
			}
			last = buff;
			pstring(2, s2); // print >>
		}
	}

    while(1);
}

