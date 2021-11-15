#include "control.h"

static void control_GPIO(void)
{
	RCC_APB2PeriphClockCmd(CONTROL_GPIO_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	//继电器要用推挽输出不然驱动能力不足
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = CONTROL_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(CONTROL_GPIO,&GPIO_InitStructure);
	
//	GPIO_ResetBits(CONTROL_GPIO,CONTROL_Pin);
	GPIO_SetBits(CONTROL_GPIO,CONTROL_Pin);
}

static void buzzer_GPIO(void)
{
	RCC_APB2PeriphClockCmd(BUZZER_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BUZZER_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(BUZZER_GPIO,&GPIO_InitStructure);
	
	GPIO_ResetBits(BUZZER_GPIO,BUZZER_Pin);
}

void BUZZER_DI(void)
{
	GPIO_SetBits(BUZZER_GPIO,BUZZER_Pin);
	SysTick_Delay_Ms(200);
	GPIO_ResetBits(BUZZER_GPIO,BUZZER_Pin);
}

void BUZZER_DIDI(void)
{
	GPIO_SetBits(BUZZER_GPIO,BUZZER_Pin);
	SysTick_Delay_Ms(200);
	GPIO_ResetBits(BUZZER_GPIO,BUZZER_Pin);
	SysTick_Delay_Ms(500);
	GPIO_SetBits(BUZZER_GPIO,BUZZER_Pin);
	SysTick_Delay_Ms(500);
	GPIO_ResetBits(BUZZER_GPIO,BUZZER_Pin);
}

void control_Init(void)
{
	control_GPIO();
	buzzer_GPIO();
}
