#ifndef _USART2_H_
#define _USART2_H_
#include "sys.h" 

#define    USART2_MAX_RECV_LEN		200			     //最大接收缓存字节数
#define    USART2_MAX_SEND_LEN		300			     //最大发送缓存字节数
extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; //接收缓冲,最大USART1_MAX_RECV_LEN字节
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; //发送缓冲,最大USART1_MAX_SEND_LEN字节
extern volatile u16 USART2_RX_STA;         	   //接收状态标记	    		             
 
void USART2_Init(u32 bound);
void USART2_printf (char *fmt, ...);
void USART2_Send_Array (u8 *array,u8 num);
 
void USART2_RX_STA_SET(void);

#endif
