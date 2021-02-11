#include "USART3.h"	  

//USART3_TX   GPIOB.10
//USART3_RX	  GPIOB.11

u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�

volatile u16 USART3_RX_STA=0;               //bit15:������ɱ�־   bit14~0:���յ�����Ч�ֽ���Ŀ 

void USART3_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		//ʱ������
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
	
		//USARTx_TX 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		//USARTx_RX 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);   
 
		//NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		 
		NVIC_Init(&NVIC_InitStructure);	 
		
		 //USARTx��ʼ�� 
		USART_InitStructure.USART_BaudRate = bound; 
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
		USART_InitStructure.USART_StopBits = USART_StopBits_1; 
		USART_InitStructure.USART_Parity = USART_Parity_No; 
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 

    //ʹ������
		USART_Init(USART3, &USART_InitStructure);     //��ʼ������
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//�������ڿ���IDEL�ж�
		USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
    USART3_printf("USART3 OK...\r\n");
}

void USART3_printf (char *fmt, ...)
{
		u32 i = 0;
		va_list arg_ptr;
		va_start(arg_ptr, fmt); 
		vsnprintf((char *)USART3_TX_BUF, USART3_MAX_SEND_LEN+1, fmt, arg_ptr);
		va_end(arg_ptr);
		while ((i < USART3_MAX_SEND_LEN) && USART3_TX_BUF[i])
		{
				USART_SendData(USART3, (u8) USART3_TX_BUF[i++]);
				while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
		}
}

void USART3_Send_Array (u8 *array,u8 num)
{
		u8 i=0;
		while (i < num)
		{
				USART_SendData(USART3, (u8) array[i++]);
				while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
		}
}

void USART3_IRQHandler(void)
{
		if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)               // �����ж�
		{
				USART_ReceiveData(USART3);                                      //��������жϱ�־
        USART3_RX_STA_SET();
        //��Ӵ�����,�����������ѯ����

		}	
		if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET)               // ��� ORE ��־,��ֹ�������ж����������ڽ����ж�ǰ��
		{
				USART_ClearFlag(USART3,USART_FLAG_ORE);
				USART_ReceiveData(USART3);
		}
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)               //�����ж� 
		{
				USART3_RX_BUF[USART3_RX_STA&0X7FFF]=USART_ReceiveData(USART3);	//��ȡ���յ�������	
				USART3_RX_STA++;
		}
}

void USART3_RX_STA_SET(void)
{
		USART3_RX_STA|=0x8000;	                                            //��ǽ��������
		if (USART3_RX_BUF[(USART3_RX_STA&0X7FFF)-2]=='\r') 
		{
				USART3_RX_BUF[(USART3_RX_STA&0X7FFF)-2]=0 ;		                  //�յ�\r\n��ɾ��\r\n�����\0
				USART3_RX_STA=USART3_RX_STA-1;                                  //\0����������һ��-2
		}
		else                                                            
		{ 
				USART3_RX_BUF[(USART3_RX_STA&0X7FFF)+1]=0 ;		                  //û�յ�\r\n��ֱ�����\0
				USART3_RX_STA=USART3_RX_STA+1;                                  //\0����������һ��+0
		}
}
 
