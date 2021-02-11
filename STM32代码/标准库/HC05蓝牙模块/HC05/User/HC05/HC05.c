#include "HC05.h"



static void HC05_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitTypeDefStructure;
	
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitTypeDefStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitTypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitTypeDefStructure);
}


void HC05_Init()
{
	HC05_GPIO_Config();
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);//模式1
	SysTick_Delay_Ms(50);
	Usart_SendString(USART2,"AT+NAME=HC05\r\n");
	SysTick_Delay_Ms(50);
	Usart_SendString(USART2,"AT+ROLE=0\r\n");
	SysTick_Delay_Ms(50);
	Usart_SendString(USART2,"AT+CMODE=1\r\n");
	SysTick_Delay_Ms(50);
	Usart_SendString(USART2,"AT+STATE?\r\n");
	SysTick_Delay_Ms(50);
	Usart_SendString(USART2,"AT+RESET\r\n");
	SysTick_Delay_Ms(50);
	/*第一步  模块上电进入模式二然后断电*/
//		GPIO_SetBits(GPIOB,GPIO_Pin_6);//模式2   
	
	/*第二步  模块上电更改波特率为BaudRate*/
//	GPIO_ResetBits(GPIOB,GPIO_Pin_6);//模式1
//	UART2_BaudRate(38400);
//	Usart_SendString(USART2,"AT+UART=BaudRate,0,0\r\n");
	
	/*第三步，查询是否更改成功*/
//	GPIO_ResetBits(GPIOB,GPIO_Pin_6);//模式1
//	UART2_BaudRate(BaudRate);
//	Usart_SendString(USART2,"AT+UART?\r\n");
}

