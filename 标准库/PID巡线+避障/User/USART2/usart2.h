#ifndef __USART2_H__
#define __USART2_H__

#include "stm32f10x.h"


#define USART_REC_LEN 200
void compare(u8 ucTemp);
void claer(void);
void USART2_Init(void);

extern u16 USART_RX_STA;
extern char USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大200个字节
#endif

