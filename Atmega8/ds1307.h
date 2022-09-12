#ifndef DS1307_H_
#define DS1307_H_

char ConvertBcdToDec(char bcdValue);
char ConvertDecToBcd(char decValue);

void DS1307Init(void);
char DS1307Read(char address);
void DS1307Write(char address, char data);

#endif /* DS1307_H_ */