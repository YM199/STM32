#include "usart2.h"	  

//USART2_TX   GPIOA.2
//USART2_RX	  GPIOA.3

u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节

volatile u16 USART2_RX_STA=0;               //bit15:接收完成标志   bit14~0:接收到的有效字节数目 

void USART2_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		//时钟设置
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
 
		//NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		 
		NVIC_Init(&NVIC_InitStructure);	 
		
		 //USARTx初始化 
		USART_InitStructure.USART_BaudRate = bound; 
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
		USART_InitStructure.USART_StopBits = USART_StopBits_1; 
		USART_InitStructure.USART_Parity = USART_Parity_No; 
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 

    //使能配置
		USART_Init(USART2, &USART_InitStructure);     //初始化串口
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启串口空闲IDEL中断
		USART_Cmd(USART2, ENABLE);                    //使能串口
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
		if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)               // 空闲中断
		{
				USART_ReceiveData(USART2);                                      //清除空闲中断标志
        USART2_RX_STA_SET();
        //添加处理函数,最好主函数查询处理

		}	
		if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)               // 检查 ORE 标志,防止开关总中断死机，放在接收中断前面
		{
				USART_ClearFlag(USART2,USART_FLAG_ORE);
				USART_ReceiveData(USART2);
		}
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)               //接收中断 
		{
				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=USART_ReceiveData(USART2);	//读取接收到的数据	
				USART2_RX_STA++;
		}
}

void USART2_RX_STA_SET(void)
{
		USART2_RX_STA|=0x8000;	                                            //标记接收完成了
		if (USART2_RX_BUF[(USART2_RX_STA&0X7FFF)-2]=='\r') 
		{
				USART2_RX_BUF[(USART2_RX_STA&0X7FFF)-2]=0 ;		                  //收到\r\n就删掉\r\n，添加\0
				USART2_RX_STA=USART2_RX_STA-1;                                  //\0不计数，这一行-2
		}
		else                                                            
		{ 
				USART2_RX_BUF[(USART2_RX_STA&0X7FFF)+1]=0 ;		                  //没收到\r\n，直接添加\0
				USART2_RX_STA=USART2_RX_STA+1;                                  //\0不计数，这一行+0
		}
}
 
