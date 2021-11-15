#include "bsp_motor.h"   
#include "bsp_AdvanceTim.h" 
#include "bsp_SysTick.h"
#include "pid.h"
void Motor_GPIO_Config(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(AIN_GPIO_CLK,ENABLE);
		GPIO_InitStructure.GPIO_Pin = AIN1_GPIO_PIN | AIN2_GPIO_PIN | BIN1_GPIO_PIN | BIN2_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(AIN_GPIO_PORT, &GPIO_InitStructure);
}
/*������һ������ ǰ��*/
void Motor_Control1(void)
{
	GPIO_SetBits(AIN_GPIO_PORT, AIN1_GPIO_PIN);
	GPIO_ResetBits(AIN_GPIO_PORT,AIN2_GPIO_PIN);
	
	GPIO_SetBits(AIN_GPIO_PORT, BIN1_GPIO_PIN);
	GPIO_ResetBits(AIN_GPIO_PORT,BIN2_GPIO_PIN);
}

//����
void Motor_Astern(void)
{
	GPIO_SetBits(AIN_GPIO_PORT, AIN2_GPIO_PIN);
	GPIO_ResetBits(AIN_GPIO_PORT,AIN1_GPIO_PIN);
	
	GPIO_SetBits(AIN_GPIO_PORT, BIN2_GPIO_PIN);
	GPIO_ResetBits(AIN_GPIO_PORT,BIN1_GPIO_PIN);
}

/*����*/
void Motor_turn(void)
{
		TIM_SetCompare2(ADVANCE_TIM,0);//С���Һ���
		TIM_SetCompare3(ADVANCE_TIM,0);//С�������
		SysTick_Delay_Ms(700);
		Motor_Astern();//����
		TIM_SetCompare1(ADVANCE_TIM,115);
		TIM_SetCompare2(ADVANCE_TIM,500);//С���Һ���
		TIM_SetCompare3(ADVANCE_TIM,500);//С�������
		SysTick_Delay_Ms(1500);
		TIM_SetCompare2(ADVANCE_TIM,0);//С���Һ���
		TIM_SetCompare3(ADVANCE_TIM,0);//С�������
	
		Motor_Control1();
		TIM_SetCompare1(ADVANCE_TIM,145);
		TIM_SetCompare2(ADVANCE_TIM,10);//С���Һ���
		TIM_SetCompare3(ADVANCE_TIM,600);//С�������
		SysTick_Delay_Ms(2000);
		TIM_SetCompare1(ADVANCE_TIM,80);
		TIM_SetCompare2(ADVANCE_TIM,600);//С���Һ���
		TIM_SetCompare3(ADVANCE_TIM,10);//С�������
		SysTick_Delay_Ms(2000);
}
/*********************************************END OF FILE**********************/
