#ifndef _USART3_H_
#define _USART3_H_
#include "sys.h" 

#define    USART3_MAX_RECV_LEN		200			     //�����ջ����ֽ���
#define    USART3_MAX_SEND_LEN		300			     //����ͻ����ֽ���
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; //���ջ���,���USART1_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; //���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
extern volatile u16 USART3_RX_STA;             //����״̬���	    		             
 
void USART3_Init(u32 bound);
void USART3_printf (char *fmt, ...);
void USART3_Send_Array (u8 *array,u8 num);
 
void USART3_RX_STA_SET(void);

#endif
