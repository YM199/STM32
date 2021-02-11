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
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);//ģʽ1
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
	/*��һ��  ģ���ϵ����ģʽ��Ȼ��ϵ�*/
//		GPIO_SetBits(GPIOB,GPIO_Pin_6);//ģʽ2   
	
	/*�ڶ���  ģ���ϵ���Ĳ�����ΪBaudRate*/
//	GPIO_ResetBits(GPIOB,GPIO_Pin_6);//ģʽ1
//	UART2_BaudRate(38400);
//	Usart_SendString(USART2,"AT+UART=BaudRate,0,0\r\n");
	
	/*����������ѯ�Ƿ���ĳɹ�*/
//	GPIO_ResetBits(GPIOB,GPIO_Pin_6);//ģʽ1
//	UART2_BaudRate(BaudRate);
//	Usart_SendString(USART2,"AT+UART?\r\n");
}

