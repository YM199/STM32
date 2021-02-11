#include "bsp_motor.h"   
#include "bsp_AdvanceTim.h" 
void Motor_GPIO_Config(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(AIN_GPIO_CLK,ENABLE);
		GPIO_InitStructure.GPIO_Pin = AIN1_GPIO_PIN | AIN2_GPIO_PIN | BIN1_GPIO_PIN | BIN2_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(AIN_GPIO_PORT, &GPIO_InitStructure);
}
/*左右轮一个方向 前进*/
void Motor_Control1(void)
{
	GPIO_SetBits(AIN_GPIO_PORT, AIN1_GPIO_PIN);
	GPIO_ResetBits(AIN_GPIO_PORT,AIN2_GPIO_PIN);
	
	GPIO_SetBits(AIN_GPIO_PORT, BIN1_GPIO_PIN);
	GPIO_ResetBits(AIN_GPIO_PORT,BIN2_GPIO_PIN);
}
/*倒车*/
void Motor_Control2(void)
{
	GPIO_SetBits(AIN_GPIO_PORT, AIN2_GPIO_PIN);
	GPIO_ResetBits(AIN_GPIO_PORT,AIN1_GPIO_PIN);
	
	GPIO_SetBits(AIN_GPIO_PORT, BIN2_GPIO_PIN);
	GPIO_ResetBits(AIN_GPIO_PORT,BIN1_GPIO_PIN);
	
	TIM_SetCompare1(ADVANCE_TIM,139);
	TIM_SetCompare1(GENERAL_TIM5,20);
	TIM_SetCompare2(GENERAL_TIM5,20);
}
/*********************************************END OF FILE**********************/
