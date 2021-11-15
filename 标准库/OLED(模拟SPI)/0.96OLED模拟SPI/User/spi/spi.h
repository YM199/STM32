#ifndef _SPI_H
#define _SPI_H

#include "stm32f10x.h"
#include "bsp_SysTick.h"

/*使用什么模式就将对应宏定义改成1*/
#define 				SPI_Mode_0         1
#define 				SPI_Mode_3				 0


#define         SPI_CS_CLK         RCC_APB2Periph_GPIOD
#define         SPI_CS_Pin				 GPIO_Pin_3
#define         SPI_CS_PORT        GPIOD

#define         SPI_SCK_CLK        RCC_APB2Periph_GPIOD
#define         SPI_SCK_Pin				 GPIO_Pin_6
#define         SPI_SCK_PORT       GPIOD

#define         SPI_MOSI_CLK       RCC_APB2Periph_GPIOD
#define         SPI_MOSI_Pin			 GPIO_Pin_7
#define         SPI_MOSI_PORT      GPIOD




#define         SPI_CS_0()         GPIO_ResetBits(SPI_CS_PORT,SPI_CS_Pin)
#define         SPI_CS_1()         GPIO_SetBits(SPI_CS_PORT,SPI_CS_Pin)


#define         SPI_SCK_0()       GPIO_ResetBits(SPI_SCK_PORT,SPI_SCK_Pin)
#define         SPI_SCK_1()       GPIO_SetBits(SPI_SCK_PORT,SPI_SCK_Pin)

#define         SPI_MOSI_0()		  GPIO_ResetBits(SPI_MOSI_PORT,SPI_MOSI_Pin)
#define         SPI_MOSI_1()		  GPIO_SetBits(SPI_MOSI_PORT,SPI_MOSI_Pin)


void Software_SPI_Init(void);
void Software_SPI_Write(u8 data);
#endif

