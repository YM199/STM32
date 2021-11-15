#ifndef _USART_
#define _USART_

#include "sys.h"

void uart3_init(u32 bound);
void USART3_IRQHandler(void);
void USART3_Send_Data(char data);
void USART3_Send_String(char *String);
#endif

