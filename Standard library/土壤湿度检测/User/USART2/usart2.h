#ifndef __USART2_H__
#define __USART2_H__

#include "stm32f10x.h"
void USART2_Init(void);
void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart2_SendString( USART_TypeDef * pUSARTx, char *str);
#endif

