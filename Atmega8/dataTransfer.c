/*
 * DataTransfer.c
 *
 * Created: 18.06.2022 8:21:43
 *  Author: SilentHill
 */
#include <xc.h>


void SendBytePortB(char dataPin, char clockPin, char data)
{
	for (int i = 0; i < 8; i++)
	{
		PORTB = (data & 0x80) ? PORTB | (1 << dataPin) : PORTB & ~(1 << dataPin);
		
		PORTB |= (1 << clockPin);
		PORTB &= ~(1 << clockPin);
		
		data <<= 1;
	}
}

void SendBytePortC(char dataPin, char clockPin, char data)
{
	for (int i = 0; i < 8; i++)
	{
		PORTC = (data & 0x80) ? PORTC | (1 << dataPin) : PORTC & ~(1 << dataPin);
		
		PORTC |= (1 << clockPin);
		PORTC &= ~(1 << clockPin);
		
		data <<= 1;
	}
}

void SendBytePortD(char dataPin, char clockPin, char data)
{
	for (int i = 0; i < 8; i++)
	{
		PORTD = (data & 0x80) ? PORTD | (1 << dataPin) : PORTD & ~(1 << dataPin);
		
		PORTD |= (1 << clockPin);
		PORTD &= ~(1 << clockPin);
		
		data <<= 1;
	}
}