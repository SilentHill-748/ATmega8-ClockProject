#include <avr/io.h>

void I2CInit(void);
void I2CStart(void);
void I2CStop(void);

void I2CWriteByte(char byte);
void I2CReadByte(char *data);

char ConvertBcdToDec(char bcd);
char ConvertDecToBcd(char dec);

void I2CInit(void)
{
	TWBR = 2; // Настройка частоты шины
	TWSR = (1 << TWPS1)|(1 << TWPS0); // Предделитель на 64
	TWCR |= (1 << TWEN); // Включение модуля TWI
}

// Функция СТАРТ
void I2CStart(void)
{
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTA);
	while(!(TWCR & (1 << TWINT)));
}

// Функция СТОП
void I2CStop(void)
{
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);
	while(TWCR & (1 << TWSTO));
}

// Функция записи данных по шине
void I2CWriteByte(char data)
{
	TWDR = data;
	TWCR = (1 << TWEN)|(1 << TWINT);
	while(!(TWCR & (1 << TWINT)));
}

// Функция чтения данных по шине
void I2CReadByte(char *data)
{
	TWCR |= (1 << TWINT);
	while(!(TWCR & (1 << TWINT)));
	*data = TWDR;
}

char ConvertBcdToDec(char bcd)
{
	char result = ((((bcd & 0xF0) >> 4) * 10) + (bcd & 0x0F));
	return result;
}

char ConvertDecToBcd(char dec)
{
	char result = (((dec / 10) << 4) + (0x0F & (dec % 10)));
	return result;
}