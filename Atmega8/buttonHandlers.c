#include <xc.h>
#include <string.h>
#include <avr/delay.h>

#include "ds1307.h"


char position = 0;
char reg_time[6] = { 0, 0, 0, 0, 0, 0 };
char isConfiguring = 0;


void SetButtonPressed(void)
{
	if (!isConfiguring)
	{
		isConfiguring = 1;
		return;
	}
	
	switch (position)
	{
		case 0:
			reg_time[0]++;
			reg_time[0] %= 3;
		break;
		case 1:
			reg_time[1]++;
			if (reg_time[0] < 2)
				reg_time[1] %= 10;
			else
				reg_time[1] %= 4;
		break;
		case 2:
			reg_time[2]++;
			reg_time[2] %= 6;
		break;
		case 3:
			reg_time[3]++;
			reg_time[3] %= 10;
		break;
		case 4:
			reg_time[4]++;
			reg_time[4] %= 6;
		break;
		case 5:
			reg_time[5]++;
			reg_time[5] %= 10;
		break;
	}
}

void ShiftButtonPressed(void)
{
	if (position == 5)
	{
		position = 0;
		return;
	}
	
	position++;
}

void ApplyButtonPressed(void)
{
	int seconds, minutes, hours;
	
	hours = reg_time[0] * 10 + reg_time[1];
	minutes = reg_time[2] * 10 + reg_time[3];
	seconds = reg_time[4] * 10 + reg_time[5];
	
	DS1307Write(0x00, seconds);
	DS1307Write(0x01, minutes);
	DS1307Write(0x02, hours);
	
	position = 0;
	isConfiguring = 0;
	memset(reg_time, 0, sizeof(reg_time));
}

void ButtonPressed(void)
{
	char buttonCode = (PIND >> 3) & 0x07;
	
	switch (buttonCode)
	{
		case 0x06:
			SetButtonPressed();
			break;
		case 0x04:
			ShiftButtonPressed();
			break;
		case 0x02:
			ApplyButtonPressed();
			break;
	}
}