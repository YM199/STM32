#include "usart1.h"


static void USART1_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_InitTypeDefStructure;
	//TX
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeDefStructure.GPIO_Pin  = GPIO_Pin_9;
	GPIO_InitTypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeDefStructure);
	
	//RX
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitTypeDefStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitTypeDefStructure);
	
	
}


static void USART1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef  USART_InitTypeDefStructure;
	
	
	USART_InitTypeDefStructure.USART_BaudRate = 115200;
	USART_InitTypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流控制
	USART_InitTypeDefStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发一起
	USART_InitTypeDefStructure.USART_Parity =USART_Parity_No;//无校验
	USART_InitTypeDefStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitTypeDefStructure.USART_WordLength = USART_WordLength_8b;//无校验位 八位
	
	USART_Init(USART1,&USART_InitTypeDefStructure);
	
	USART_Cmd(USART1,ENABLE);
}


/*****************  发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, u8 *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*串口引脚初始化必须放在串口初始化之前，否者会出现异常数据0XFF*/
void USART1_Init(void)
{
	USART1_Config();
	USART1_GPIO_Config();
}

