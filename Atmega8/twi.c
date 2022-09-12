#include <xc.h>

void TwiInit(void)
{
	TWBR = 0x00;
}

void TwiStart(void)
{
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
}

void TwiStop(void)
{
	TWCR = (1 << TWINT)|(1 << TWSTO)|(1 << TWEN);
}

void TwiSendByte(char data)
{
	TWDR = data;
	TWCR = (1 << TWINT)|((1 << TWEN));
	while(!(TWCR & (1 << TWINT)));
}

char TwiRead()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
	return TWDR;
}

char TwiReadLastByte(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
	return TWDR;
}