#include "spi.h"

void Software_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(SPI_CS_CLK|SPI_SCK_CLK|SPI_MOSI_CLK,ENABLE);
	
	/*CS*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_CS_PORT,&GPIO_InitStructure);
	/*SCK*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_SCK_PORT,&GPIO_InitStructure);
	/*MOSI*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_MOSI_PORT,&GPIO_InitStructure);	
	
	
	SPI_CS_1();
	SPI_SCK_1();
	SPI_MOSI_1();
}

#if SPI_Mode_0
/*
Mode_0
MSB先行
data :待写入的数据
*/
void Software_SPI_Write(u8 data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		SPI_SCK_0();
		SysTick_Delay_Us(10);
		if(data & 0x80)
		{
			SPI_MOSI_1();
		}
		else
		{
			SPI_MOSI_0();
		}
		data <<= 1;
		SPI_SCK_1();
		SysTick_Delay_Us(10);		
	}
}




#elif SPI_Mode_3
/*

Mode_3
MSB先行
data :待写入的数据
*/

void Software_SPI_Write_Read(u8 data)
{
	u8 i;
	SPI_SCK_1();
	SysTick_Delay_Us(10);
	for(i=0;i<8;i++)
	{
		SPI_SCK_0();
		SysTick_Delay_Us(10);	
		if(data & 0x80)
		{
			SPI_MOSI_1();
		}
		else
		{
			SPI_MOSI_0();
		}
		data <<= 1;
		SPI_SCK_1();
		SysTick_Delay_Us(10);	
	}

}
#endif

