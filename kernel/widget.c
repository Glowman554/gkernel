#include "widget.h"
#include "console.h"

static char* video = (char*) 0xb8000;

int x = 2;

void pstring(char s[]);

void create_error(int code){
	clrscr();
	for(int i = 0; i < 80; i++){
		video[2 * (10 * 80 + i)] = 219;
		video[2 * (10 * 80 + i) + 1] = 0x4;
		video[2 * (15 * 80 + i)] = 219;
		video[2 * (15 * 80 + i) + 1] = 0x4;
	}
	
	for(int i = 10; i < 16; i++){
		video[2 * (i * 80 + 0)] = 219;
		video[2 * (i * 80 + 0) + 1] = 0x4;
		video[2 * (i * 80 + 79)] = 219;
		video[2 * (i * 80 + 79) + 1] = 0x4;
	}
	
	char s1[] = "Exception ";
	int i;
    // C-Strings haben ein Nullbyte als Abschluss
    for (i = 0; s1[i] != '\0'; i++) {
    	video[2 * (12 * 80 + i+2)] = s1[i];
		video[2 * (12 * 80 + i+2) + 1] = 0x4;
		x++;
    }
    
    setx(x);
    sety(12);
    setcolor(0x4);
    kputn(code, 10);
    
    int offset = x+1;
    
    if(code > 9){
    	offset = x+2;
	}
    
	char s2[] = ", Kernel angehalten";
	for (i = 0; s2[i] != '\0'; i++) {
    	video[2 * (12 * 80 + i+offset)] = s2[i];
		video[2 * (12 * 80 + i+offset) + 1] = 0x4;
		x++;
    }
    
    switch(code){
    	case 0x0:
    		pstring("Divide by Zero");
    		break;
    	case 0x1:
    		pstring("Debug");
    		break;
    	case 0x2:
    		pstring("Non Maskable Interrupt");
    		break;
    	case 0x3:
    		pstring("Breakpoint");
    		break;
    	case 0x4:
    		pstring("Overflow");
    		break;
    	case 0x5:
    		pstring("Bound Range");
    		break;
    	case 0x6:
    		pstring("Invalid Opcode");
    		break;
    	case 0x7:
    		pstring("Device Not Available");
    		break;
    	case 0x8:
    		pstring("Double Fault");
    		break;
    	case 0x9:
    		pstring("Coprocessor Segment Overrun");
    		break;
    	case 0xa:
    		pstring("Invalid TSS");
    		break;
    	case 0xb:
    		pstring("Segment not Present");
    		break;
    	case 0xc:
    		pstring("Stack Fault");
    		break;
    	case 0xd:
    		pstring("General Protection");
    		break;
    	case 0xe:
    		pstring("Page Fault");
    		break;
    	case 0x10:
    		pstring("x87 Floating Point");
    		break;
    	case 0x11:
    		pstring("Alignment Check");
    		break;
    	case 0x12:
    		pstring("Machine Check");
    		break;
    	case 0x13:
    		pstring("SIMD Floating Point");
    		break;
    	case 0x1e:
    		pstring("Security-sensitive event in Host");
    		break;
    	default:
    		pstring("Reserviert");
    		break;
	}
    
    
    
	while(1) {
	    // Prozessor anhalten
		asm volatile("cli; hlt");
	}
	
}

void pstring(char s[]){
	int i;
	for (i = 0; s[i] != '\0'; i++) {
    	video[2 * (13 * 80 + i+2)] = s[i];
		video[2 * (13 * 80 + i+2) + 1] = 0x4;
    }
}
