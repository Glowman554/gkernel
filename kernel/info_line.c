#include "info_line.h"
#include "console.h"

static char* video = (char*) 0xb8000;

void drawinfo(int tcount)
{
	int i,x = 0;
	for(i = 0; i < 80; i++){
		video[2 * (24 * 80 + i)] = 0;
		video[2 * (24 * 80 + i) + 1] = 0x7f;
	}
	
	char s1[] = "Running Processes:  ";
    // C-Strings haben ein Nullbyte als Abschluss
    for (i = 0; s1[i] != '\0'; i++) {
    	video[2 * (24 * 80 + i+2)] = s1[i];
		video[2 * (24 * 80 + i+2) + 1] = 0x7f;
		x++;
    }
    int ox = getx();
    int oy = gety();
    setscrole(0);
    
    setx(x);
    sety(24);
    setcolor(0x7f);
    kputn(tcount,10);
    
    setscrole(1);
    setx(ox);
    sety(oy);
}
