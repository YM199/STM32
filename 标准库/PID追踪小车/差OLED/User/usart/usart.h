#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include "usart_a.h"
#include "usart_b.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define DEBUG_USARTx USART1
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void determine_interrupt(void);
void obtain_coordinate(char* data);
#endif /* __USART_H */

