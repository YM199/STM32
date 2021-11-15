#include "usart2.h"	  

//USART2_TX   GPIOA.2
//USART2_RX	  GPIOA.3

u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�

volatile u16 USART2_RX_STA=0;               //bit15:������ɱ�־   bit14~0:���յ�����Ч�ֽ���Ŀ 

void USART2_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		//ʱ������
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	
		//USARTx_TX 
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		//USARTx_RX 
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);  
 
		//NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
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
		USART_Init(USART2, &USART_InitStructure);     //��ʼ������
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
		USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�������ڿ���IDEL�ж�
		USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
    USART2_printf("USART2 OK...\r\n");
}

void USART2_printf (char *fmt, ...)
{
		u32 i = 0;
		va_list arg_ptr;
		va_start(arg_ptr, fmt); 
		vsnprintf((char *)USART2_TX_BUF, USART2_MAX_SEND_LEN+1, fmt, arg_ptr);
		va_end(arg_ptr);
		while ((i < USART2_MAX_SEND_LEN) && USART2_TX_BUF[i])
		{
				USART_SendData(USART2, (u8) USART2_TX_BUF[i++]);
				while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
		}
}

void USART2_Send_Array (u8 *array,u8 num)
{
		u8 i=0;
		while (i < num)
		{
				USART_SendData(USART2, (u8) array[i++]);
				while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
		}
}

void USART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)               // �����ж�
		{
				USART_ReceiveData(USART2);                                      //��������жϱ�־
        USART2_RX_STA_SET();
        //��Ӵ�����,�����������ѯ����

		}	
		if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)               // ��� ORE ��־,��ֹ�������ж����������ڽ����ж�ǰ��
		{
				USART_ClearFlag(USART2,USART_FLAG_ORE);
				USART_ReceiveData(USART2);
		}
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)               //�����ж� 
		{
				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=USART_ReceiveData(USART2);	//��ȡ���յ�������	
				USART2_RX_STA++;
		}
}

void USART2_RX_STA_SET(void)
{
		USART2_RX_STA|=0x8000;	                                            //��ǽ��������
		if (USART2_RX_BUF[(USART2_RX_STA&0X7FFF)-2]=='\r') 
		{
				USART2_RX_BUF[(USART2_RX_STA&0X7FFF)-2]=0 ;		                  //�յ�\r\n��ɾ��\r\n�����\0
				USART2_RX_STA=USART2_RX_STA-1;                                  //\0����������һ��-2
		}
		else                                                            
		{ 
				USART2_RX_BUF[(USART2_RX_STA&0X7FFF)+1]=0 ;		                  //û�յ�\r\n��ֱ�����\0
				USART2_RX_STA=USART2_RX_STA+1;                                  //\0����������һ��+0
		}
}
 
