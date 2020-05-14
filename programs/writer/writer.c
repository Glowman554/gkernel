#include <stdint.h>
#include "syslib.h"


void _start(void)
{	
	pstring(0xf, "Press any key  to start writer");
	//ls();
	getchar();
	
	int x = 0;
	int y = 0;
	
	clrscr();
	for(int x = 0; x < 80; x++){
		sety(0);
		setx(x);
		pchar(0xa, 219);
		sety(22);
		setx(x);
		pchar(0xa, 219);
	}
	
	for(int y = 0; y < 22; y++){
		sety(y);
		setx(0);
		pchar(0xa, 219);
		sety(y);
		setx(79);
		pchar(0xa, 219);
	}
	
	sety(0);
	setx(80/2-(6/2));
	pstring(0xa0, "Writer");
	
	char buff;
	
	while(1){
		setx(x + 2);
		sety(y + 2);
		reset_tick();
		while(get_tick() < 5);
		//10 -> enter; 8 -> backspace
		buff = getchar();
		
		switch(buff){
			case 10:
				y++;
				x = 0;
				break;
			case 8:
				setx(x + 2 - 1);
				pchar(0xf, ' ');
				if(x == 0){
					y--;
					x = 74;
				} else {
					x--;
				}
		}
		
		if(buff != 10 && buff != 8){
			pchar(0xf, buff);
			x++;
		}
		
		if(x == 75){
			y++;
			x = 0;
		}
		
		if(y == 20){
			setx(80/2-(14/2));
			sety(22);
			pstring(0xa4, "No more space!");
			while(1);
		}
	}
	
    while(1);
}

