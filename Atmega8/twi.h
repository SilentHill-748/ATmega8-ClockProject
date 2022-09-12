#ifndef TWI_H_
#define TWI_H_

void TwiInit(void);

void TwiStart(void);
void TwiStop(void);

void TwiSendByte(char data);
char TwiRead();
char TwiReadLastByte(void);

#endif /* TWI_H_ */