#include "LED.h"

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitTypeDefStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDefStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitTypeDefStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitTypeDefStructure.GPIO_Pin= GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5;
	GPIO_Init(GPIOB,&GPIO_InitTypeDefStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
}


void compare(u8 *str)
{
	if(*(str+1)=='F')
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		Usart_SendString(USART1,"LED IS OFF\r\n");
	}
	else if(*(str+1)=='N')
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		Usart_SendString(USART1,"LED IS ON\r\n");
	}
}


