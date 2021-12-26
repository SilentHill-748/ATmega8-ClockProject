#include <inttypes.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

char digitNums[10][5] = {			// Массив всех чисел в виде 16-ых кодов.
	{0x83,0x7D,0x7D,0x83,0xFF},	//0
	{0xDF,0xBD,0x01,0xFD,0xFF},	//1
	{0xB3,0x6D,0x6D,0x9D,0xFF},	//2
	{0xBB,0x6D,0x6D,0x93,0xFF},	//3
	{0x0F,0xEF,0xEF,0x01,0xFF},	//4
	{0x0D,0x6D,0x6D,0x73,0xFF},	//5
	{0x83,0x6D,0x6D,0xB3,0xFF},	//6
	{0x7F,0x71,0x6F,0x1F,0xFF},	//7
	{0x93,0x6D,0x6D,0x93,0xFF},	//8
	{0x9B,0x6D,0x6D,0x83,0xFF},	//9
};

char time[]			= {0,0,0,0,0,0};	// Массив времени, которое надо показывать.
char dataRows[32];						// Массив кадров, которые выводятся на дисплей.
char reg_time[6]	= {0,0,0,0,0,0};	// Регистр настроенного времени. Хранит вручную настроенное время.
char shift			= 0;				// Переменная смещения настраимого числа.
char isConfig		= 0;				// Флаг конфигурации. Если 1 - выводит в режиме настройки времени.

void Framing(void);
void ConfigFraming(void);
void PrintTime(void);

// DS1307
void SetTime(char h, char m, char s);
void GetTime(void);

// Handlers
void SetChangeButton_Pressed(void);
void ShiftButton_Pressed(void);
void ApplySetButton_Pressed(void);
void ButtonPress(void);

// Bus
void SPIInit(void);
void SPISendByte(unsigned char byte);

void Framing()
{
	int i, j, k;
	for (i = 0, j = 0, k = 0; i < 32; i++)
	{
		if ((i != 10) && (i != 21))
		{
			dataRows[i] = digitNums[time[j]][k];
			k++;
			
			if (k == 5)
			{
				j++;
				k = 0;
			}
		}
		else
		{
			dataRows[i] = 0b10111101;
		}
	}
}

void ConfigFraming()
{
	int i, j, k;
	for (i = 0, j = 0, k = 0; i < 32; i++)
	{
		if (j <= shift)
		{
			dataRows[i] = digitNums[reg_time[j]][k];
			k++;
			
			if (k == 5)
			{
				k = 0;
				j++;
			}
		}
		else
		{
			dataRows[i] = 0xFF;
		}
	}
}

void SetTime(char h, char m, char s)
{
	DS1307Write(0x00, ConvertDecToBcd(s));
	DS1307Write(0x01, ConvertDecToBcd(m));
	DS1307Write(0x02, ConvertDecToBcd(h));
}

void GetTime()
{
	unsigned char temp, s, m, h;
	DS1307Read(0x00,&temp);
	s = ConvertBcdToDec(temp);
	DS1307Read(0x01,&temp);
	m = ConvertBcdToDec(temp);
	DS1307Read(0x02,&temp);
	h = ConvertBcdToDec(temp);
	
	time[0] = h / 10;
	time[1] = h % 10;
	time[2] = m / 10;
	time[3] = m % 10;
	time[4] = s / 10;
	time[5] = s % 10;
}

void SetChangeButton_Pressed()
{
	if (!isConfig)
	{
		isConfig = 1;
		return;
	}
	else
	{
		switch (shift)
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
}

void ShiftButton_Pressed()
{
	if (shift == 5) {
		shift = 0;
		return;
	}
	
	shift++;
}

void ApplySetButton_Pressed()
{
	int s, m, h;
	h = ((reg_time[0] * 10) + reg_time[1]);
	m = ((reg_time[2] * 10) + reg_time[3]);
	s = ((reg_time[4] * 10) + reg_time[5]);
	
	SetTime(h, m, s);
	
	shift = 0;
	isConfig = 0;
	memset(reg_time, 0, sizeof(reg_time));
	
	GetTime();
	Framing();
	PrintTime();
}

void SendByte(unsigned int byte)
{
	for (int i = 0; i < 8; i++)
	{
		PORTC = (byte & 128) ? PORTC | (1 << PORTC1) : PORTC & ~(1 << PORTC1);
		PORTC |= (1 << PORTC0);
		PORTC &= ~(1 << PORTC0);
		byte <<= 1;
	}
}

void PrintTime(void)
{
	for (int i = 0; i < 8; i++)
	{
		SendByte(1 << i);

		for (int j = 0; j < 4; j++)
		{
			SendByte(dataRows[j * 8 + i]);
		}

		PORTC |= (1 << PORTC2);
		PORTC &= ~(1 << PORTC2);
	}
}

void ButtonPress()
{
	unsigned char pind;
	pind = (PIND >> 3) & 0x07;
	while ((pind == 0x07)); // ожидание отпуска кнопки.
	
	if (pind == 0x06)
	{
		SetChangeButton_Pressed();
	}
	else if (pind == 0x04)
	{
		ShiftButton_Pressed();
	}
	else if (pind == 0x02)
	{
		ApplySetButton_Pressed();
	}
}

ISR(INT1_vect)
{
	cli();
	GICR = 0x00;
	ButtonPress();
	GICR = 0xC0;
	sei();
}

ISR(INT0_vect)
{
	GetTime();
}

int main()
{
	DDRD = 0x00;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0b11100011;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTB = 0x00;
	PORTD |= (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5);

	// Установка импульса с пина SOUT раз в секунду и включение вывода этого импульса.
	DS1307Write(0x07, 0b00010000);
	
	I2CInit(); // Инициализация шины I2C
	
	GICR |= (1 << INT1) | (1 << INT0);
	MCUCR = (1 << ISC01) | (1 << ISC11); // вкл прерывание на INT0-1 при лог. 0 на порту.
	sei();
	
	while (1)
	{
		if (isConfig)
		{
			ConfigFraming();	// Установка кадров режима настройки часов.
		}
		else
		{
			Framing();			// Установка кадров обычного режима.
		}
		
		PrintTime();			// Вывод часов.
	};
	return 0;
}