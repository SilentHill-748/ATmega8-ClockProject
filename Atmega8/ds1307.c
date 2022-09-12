#include "twi.h"

#define DS1307			0b11010000
#define DS1307R			(DS1307 + 1)
#define DS1307W			DS1307
#define DS1307_32kHz	0b11010011


unsigned char ConvertBcdToDec(unsigned char bcdValue)
{
	char result = ((((bcdValue & 0xF0) >> 4) * 10) + (bcdValue & 0x0F));
	return result;
}

unsigned char ConvertDecToBcd(unsigned char decValue)
{
	char result = (((decValue / 10) << 4) + (0x0F & (decValue % 10)));
	return result;
}

void DS1307Init()
{
	TwiStart();
	TwiSendByte(DS1307W);
	TwiSendByte(0x07);
	TwiSendByte(0b00010000);
}

unsigned char DS1307Read(unsigned char addr)
{
	TwiStart();
	TwiSendByte(DS1307W);
	TwiSendByte(addr);
	TwiStart();
	TwiSendByte(DS1307R);
	
	return ConvertBcdToDec(TwiRead());
}

void DS1307Write(unsigned char addr, unsigned char data)
{
	TwiStart();
	TwiSendByte(DS1307W);
	TwiSendByte(addr);
	TwiSendByte(ConvertDecToBcd(data));
}