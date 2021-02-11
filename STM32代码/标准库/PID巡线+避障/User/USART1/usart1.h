#ifndef __USART1__H
#define __USART1__H

#include "stm32f10x.h"
#include <stdio.h>
void USART1_Init(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
#endif
