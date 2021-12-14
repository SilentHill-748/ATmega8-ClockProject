#define F_CPU 16000000L
#define PC1 PORTC1

#include <avr/io.h>
#include "avr/delay.h"


void test(){
	
}

void init(){
	DDRB = 0b111111111;
	DDRC = 0b111111111;
	DDRD = 0b111111111;
}

int main(void)
{
	init();
	
    while (1) 
    {
		
		test();
    }
}

