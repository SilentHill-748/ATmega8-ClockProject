void DS1307Read(char address, char *data);
void DS1307Write(char address, char data);

// ������� ������ ������ �� DS1307
void DS1307Read(char address, char *data)
{
	I2CStart();
	I2CWriteByte(0b11010000);
	I2CWriteByte(address);
	I2CStart();
	I2CWriteByte(0b11010001);
	I2CReadByte(data);
	I2CStop();
}

// ������� ������ ������ � DS1307
void DS1307Write(char address, char data)
{
	I2CStart();
	I2CWriteByte(0b11010000);
	I2CWriteByte(address);
	I2CWriteByte(data);
	I2CStop();
}