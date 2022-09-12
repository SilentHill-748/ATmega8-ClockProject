#include <xc.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#include "dataTransfer.h"
#include "ds1307.h"
#include "buttonHendlers.h"

char nums[10][5] = 
{
	{ 0x3E, 0x41, 0x41, 0x3E, 0x00 },
	{ 0x04, 0x42, 0x7F, 0x40, 0x00 },
	{ 0x62, 0x51, 0x49, 0x46, 0x00 },
	{ 0x22, 0x41, 0x49, 0x36, 0x00 },
	{ 0x18, 0x14, 0x12, 0x7F, 0x00 },
	{ 0x2F, 0x49, 0x49, 0x31, 0x00 },
	{ 0x3E, 0x49, 0x49, 0x32, 0x00 },
	{ 0x03, 0x71, 0x09, 0x07, 0x00 },
	{ 0x36, 0x49, 0x49, 0x36, 0x00 },
	{ 0x26, 0x49, 0x49, 0x3E, 0x00 }
};

char rows[32], seconds, minutes, hours;

char time[6] = { 9, 9, 9, 9, 9, 9 };
	
void SetFraming()
{
	uint8_t k = 0, j = 0;
	for (uint8_t i = 0; i < 32; i++)
	{
		if (i == 10 || i == 21)
		{
			continue;
		}
		
		if (isConfiguring)
			rows[i] = nums[(uint8_t)reg_time[k]][j];
		else
			rows[i] = nums[(uint8_t)time[k]][j];
			
		j++;
		
		if (j == 5)
		{
			k++;
			j = 0;
		}
	}
}

void UpdateTime()
{	
	char ss, mm, hh;
	
	ss = DS1307Read(0x00);
	mm = DS1307Read(0x01);
	hh = DS1307Read(0x02);
	
	time[0] = hh / 10;
	time[1] = hh % 10;
	time[2] = mm / 10;
	time[3] = mm % 10;
	time[4] = ss / 10;
	time[5] = ss % 10;
}

void SetupPins()
{
	DDRD = 0b11000011;
	DDRB = 0xFF;
	DDRC = 0xFF;
	
	PORTC = 0x00;
	PORTD = (1 << 3) | (1 << 4) | (1 << 5);
	PORTB = 0x00;
}

void SetupInterrups()
{
	sei();
	GICR = /*(1 << INT0) |*/ (1 << INT1);
	MCUCR = (1 << ISC11) | (1 << ISC00) | (1 << ISC01);
}

void Initial()
{
	SetupPins();
	TwiInit();
	DS1307Init();
	SetupInterrups();
}

void PrintTime()
{
	for (int i = 0; i < 8; i++)
	{
		SendBytePortB(0, 1, 1 << i);
		
		for (int j = 0; j < 4; j++)
		{
			SendBytePortC(0, 1, rows[j * 8 + i]);
		}
		
		PORTC |= (1 << PINC2);
		PORTC &= ~((1 << PINC2));
		
		PORTD |= (1 << PIND6);
		PORTD &= ~(1 << PIND6);
	}
}

//ISR (INT0_vect)
//{
	//UpdateTime();
//}

ISR (INT1_vect)
{
	_delay_ms(10);
	ButtonPressed();
}

int main(void)
{
	Initial();
	
    while(1)
    {
		UpdateTime();
		SetFraming();
		PrintTime();
    }
}