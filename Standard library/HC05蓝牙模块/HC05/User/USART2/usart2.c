#include "usart2.h"

static void USART2_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_InitTypeDefStructure;
	//TX
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeDefStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitTypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeDefStructure);
	
	//RX
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitTypeDefStructure.GPIO_Pin  = GPIO_Pin_3;
	
	GPIO_Init(GPIOA,&GPIO_InitTypeDefStructure);
}


static void USART2_Config(void)
{
	/*时钟初始化放在最前面，*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	NVIC_InitTypeDef   NVIC_InitTypeDefStructure;

			/*中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDefStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitTypeDefStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	NVIC_InitTypeDefStructure.NVIC_IRQChannelSubPriority = 1;//子优先级
	NVIC_InitTypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitTypeDefStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能串口接收中断
	
	USART_InitTypeDef  USART_InitTypeDefStructure;
	USART_InitTypeDefStructure.USART_BaudRate = 115200;
	USART_InitTypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流控制
	USART_InitTypeDefStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发一起
	USART_InitTypeDefStructure.USART_Parity =USART_Parity_No;//无校验
	USART_InitTypeDefStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitTypeDefStructure.USART_WordLength = USART_WordLength_8b;//无校验位 八位
	
	USART_Init(USART2,&USART_InitTypeDefStructure);
	
	USART_Cmd(USART2,ENABLE);
}


/*串口引脚初始化必须放在串口初始化之前，否者会出现异常数据0XFF*/
void USART2_Init(void)
{
	USART2_Config();
	USART2_GPIO_Config();
}

