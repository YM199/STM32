#ifndef _SPI_H
#define _SPI_H

#include "stm32f10x.h"
#include "bsp_SysTick.h"

/*使用什么模式就将对应宏定义改成1*/
#define 				SPI_Mode_0         1
#define 				SPI_Mode_1				 0
#define 				SPI_Mode_2				 0
#define 				SPI_Mode_3				 0


#define         SPI_CS_CLK         RCC_APB2Periph_GPIOC
#define         SPI_CS_Pin				 GPIO_Pin_0
#define         SPI_CS_PORT        GPIOC

#define         SPI_SCK_CLK        RCC_APB2Periph_GPIOA
#define         SPI_SCK_Pin				 GPIO_Pin_5
#define         SPI_SCK_PORT       GPIOA

#define         SPI_MOSI_CLK       RCC_APB2Periph_GPIOA
#define         SPI_MOSI_Pin			 GPIO_Pin_7
#define         SPI_MOSI_PORT      GPIOA

#define         SPI_MISO_CLK       RCC_APB2Periph_GPIOA
#define         SPI_MISO_Pin			 GPIO_Pin_6
#define         SPI_MISO_PORT      GPIOA




#define         SPI_CS_0()         GPIO_ResetBits(SPI_CS_PORT,SPI_CS_Pin)
#define         SPI_CS_1()         GPIO_SetBits(SPI_CS_PORT,SPI_CS_Pin)


#define         SPI_SCK_0()       GPIO_ResetBits(SPI_SCK_PORT,SPI_SCK_Pin)
#define         SPI_SCK_1()       GPIO_SetBits(SPI_SCK_PORT,SPI_SCK_Pin)

#define         SPI_MOSI_0()		  GPIO_ResetBits(SPI_MOSI_PORT,SPI_MOSI_Pin)
#define         SPI_MOSI_1()		  GPIO_SetBits(SPI_MOSI_PORT,SPI_MOSI_Pin)



#define   			SPI_MISO()				GPIO_ReadInputDataBit(SPI_MISO_PORT,SPI_MISO_Pin)

void Software_SPI_Init(void);
u8 Software_SPI_Write_Read(u8 data);
#endif

