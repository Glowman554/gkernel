#include <stdint.h>
#include "cmos.h"

void _start(void)
{
	register uint32_t input asm("ebx");
	
	char s1[16] = "Prees h for help";
	char s2[2] = ">>";
	char s3[11] = "r -> reboot";
	char s4[9] = "h -> help";
	char s5[10] = "a -> about";
	char s6[26] = "Programiert von glowman434";
	char s7[49] = "Vielen dank an das tolle tutorial von lowlevel.eu";
	char s8[9] = "t -> time";
	
	char buff;
	char last;
	
	asm("int $0x30" : : "a" (0), "b" (10)); // return
	
    for (int i = 0; i < 16; i++) {
        asm("int $0x30" : : "a" (0), "b" (s1[i])); // print init text
    }
	asm("int $0x30" : : "a" (0), "b" (10)); // return
    for (int i = 0; i < 2; i++) {
        asm("int $0x30" : : "a" (0), "b" (s2[i])); // print >>
    }
    
    while(1){
    	asm("int $0x30" : : "a" (1));
    	buff = input;
    	if(buff != last){
    		asm("int $0x30" : : "a" (0), "b" (buff)); // print buff
    		asm("int $0x30" : : "a" (0), "b" (10)); // return
    		switch(buff){
    			case 'r':
	        		asm("int $0x30" : : "a" (2)); // reboot
	        		break;
	        	case 'h':
	        		for (int i = 0; i < 11; i++) {
        				asm("int $0x30" : : "a" (0), "b" (s3[i])); // print help text 1
    				}
					asm("int $0x30" : : "a" (0), "b" (10)); // return
					for (int i = 0; i < 9; i++) {
        				asm("int $0x30" : : "a" (0), "b" (s4[i])); // print help text 2
    				}
					asm("int $0x30" : : "a" (0), "b" (10)); // return
					for (int i = 0; i < 10; i++) {
        				asm("int $0x30" : : "a" (0), "b" (s5[i])); // print help text 3
    				}
					asm("int $0x30" : : "a" (0), "b" (10)); // return
					for (int i = 0; i < 9; i++) {
        				asm("int $0x30" : : "a" (0), "b" (s8[i])); // print help text 4
    				}
					asm("int $0x30" : : "a" (0), "b" (10)); // return
					break;
				case 'a':
					for (int i = 0; i < 26; i++) {
        				asm("int $0x30" : : "a" (0), "b" (s6[i])); // print about text 1
    				}
					asm("int $0x30" : : "a" (0), "b" (10)); // return
					for (int i = 0; i < 49; i++) {
        				asm("int $0x30" : : "a" (0), "b" (s7[i])); // print about text 2
    				}
					asm("int $0x30" : : "a" (0), "b" (10)); // return
					break;
				case 't':
					print_time();
					break;
			}
			last = buff;
			for (int i = 0; i < 2; i++) {
    		    asm("int $0x30" : : "a" (0), "b" (s2[i])); // print >>
    		}
		}
	}

    while(1);
}

