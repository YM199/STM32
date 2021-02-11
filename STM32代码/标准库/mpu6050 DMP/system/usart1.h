#ifndef _USART1_H_
#define _USART1_H_
#include "sys.h" 

#define    USART1_MAX_RECV_LEN		200			     //�����ջ����ֽ���
#define    USART1_MAX_SEND_LEN		300			     //����ͻ����ֽ���
extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; //���ջ���,���USART1_MAX_RECV_LEN�ֽ�
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; //���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
extern volatile u16 USART1_RX_STA;         	   //����״̬���	    		             
 
void USART1_Init(u32 bound);
void USART1_printf (char *fmt, ...);
void USART1_Send_Array (u8 *array,u8 num);
 
void USART1_RX_STA_SET(void);

#endif
