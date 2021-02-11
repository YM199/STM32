#include "w25q64.h"

void Read_ID(void)
{
	u8 ID;
	//printf("ÄãºÃ");
	SPI_CS_0();
	
	Software_SPI_Write_Read(0xAB);
	
	Software_SPI_Write_Read(0xff);

	Software_SPI_Write_Read(0xff);

	Software_SPI_Write_Read(0xff);

	ID = Software_SPI_Write_Read(0xff);

	SPI_CS_1();
	printf("%x\r\n",ID);
	
}

