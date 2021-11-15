#ifndef _USART3_H_
#define _USART3_H_
#include "sys.h" 

#define    USART3_MAX_RECV_LEN		200			     //最大接收缓存字节数
#define    USART3_MAX_SEND_LEN		300			     //最大发送缓存字节数
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; //接收缓冲,最大USART1_MAX_RECV_LEN字节
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; //发送缓冲,最大USART1_MAX_SEND_LEN字节
extern volatile u16 USART3_RX_STA;             //接收状态标记	    		             
 
void USART3_Init(u32 bound);
void USART3_printf (char *fmt, ...);
void USART3_Send_Array (u8 *array,u8 num);
 
void USART3_RX_STA_SET(void);

#endif
