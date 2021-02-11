#ifndef _OLED_H
#define _OLED_H
#include "stm32f10x.h"
#include "spi.h"
#include "bsp_SysTick.h"

#define         OLED_RES_CLK        RCC_APB2Periph_GPIOD
#define         OLED_RES_Pin				GPIO_Pin_4
#define         OLED_RES_PORT       GPIOD

#define         OLED_DC_CLK         RCC_APB2Periph_GPIOD
#define         OLED_DC_Pin				  GPIO_Pin_5
#define         OLED_DC_PORT        GPIOD

#define         OLED_RES_1()        GPIO_SetBits(OLED_RES_PORT,OLED_RES_Pin)
#define         OLED_RES_0()        GPIO_ResetBits(OLED_RES_PORT,OLED_RES_Pin)

#define         OLED_DC_Data()      GPIO_SetBits(OLED_DC_PORT,OLED_DC_Pin)
#define         OLED_DC_Cmd()       GPIO_ResetBits(OLED_DC_PORT,OLED_DC_Pin)

void OLED_Init(void);
void OLED_Clear(void);
void OLED_8x16Str(u8 x,u8 y,char *ch);
void OLED_ShowCHinese(u8 x,u8 y,u8 length);
#endif

