#ifndef _USART2_H_
#define _USART2_H_
#include "sys.h" 

#define    USART2_MAX_RECV_LEN		200			     //�����ջ����ֽ���
#define    USART2_MAX_SEND_LEN		300			     //����ͻ����ֽ���
extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; //���ջ���,���USART1_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; //���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
extern volatile u16 USART2_RX_STA;         	   //����״̬���	    		             
 
void USART2_Init(u32 bound);
void USART2_printf (char *fmt, ...);
void USART2_Send_Array (u8 *array,u8 num);
 
void USART2_RX_STA_SET(void);

#endif
