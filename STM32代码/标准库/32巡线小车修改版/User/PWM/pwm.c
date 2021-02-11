#include "pwm.h"

//TIM5 IO口配置
static void TIM5_GPIO_Config(void)
{
	GPIO_InitTypeDef TIM5_Struct;//GPIO结构体
	RCC_APB2PeriphClockCmd(TIM5_GPIO_CLK, ENABLE);//打开时钟
	
	
	TIM5_Struct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	TIM5_Struct.GPIO_Pin = TIM5_CH1_Pin | TIM5_CH2_Pin | TIM5_CH3_Pin | TIM5_CH4_Pin;
	TIM5_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(TIM5_GPIO,&TIM5_Struct);	
}


static void TIM5_Config()
{
	//开启定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	//初始化时基结构体初始化
	TIM_TimeBaseInitTypeDef TIM5_TimeBaseStruct;
	TIM5_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频 1分频
	TIM5_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;//自下而上
	TIM5_TimeBaseStruct.TIM_Period = GENERAL_TIM5_Period;
	TIM5_TimeBaseStruct.TIM_Prescaler = GENERAL_TIM5_PSC;
	TIM5_TimeBaseStruct.TIM_RepetitionCounter = 0;//重复计数器 只存在于高级定时器 
	TIM_TimeBaseInit(GENERAL_TIM5, &TIM5_TimeBaseStruct);
	//定时器输出比较结构体初始化
	TIM_OCInitTypeDef TIM5_OCStruct;
	//PWM1模式
	TIM5_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
	//比较输出使能
	TIM5_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	//互补输出不使能
	TIM5_OCStruct.TIM_OutputNState = TIM_OutputNState_Disable;
	//输出比较通道高电平有效
	TIM5_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	//互补输出通道电平极性配置
	TIM5_OCStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// 输出通道空闲电平极性配置
	TIM5_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	//互补输出通道空闲电平极性配置
	TIM5_OCStruct.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	
	// 输出比较通道 1
	TIM5_OCStruct.TIM_Pulse = 0;//设置通道一的初始占空比
	TIM_OC1Init(GENERAL_TIM5,&TIM5_OCStruct);//初始化CH1通道
	TIM_OC1PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);//使能预装载寄存器
	
	//输出比较通道2
	TIM5_OCStruct.TIM_Pulse = 0;
	TIM_OC2Init(GENERAL_TIM5,&TIM5_OCStruct);
	TIM_OC2PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);
	
	//输出比较通道3
	TIM5_OCStruct.TIM_Pulse = 0;
	TIM_OC3Init(GENERAL_TIM5,&TIM5_OCStruct);
	TIM_OC3PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);
	
	//输出比较通道4
	TIM5_OCStruct.TIM_Pulse = 0;
	TIM_OC4Init(GENERAL_TIM5,&TIM5_OCStruct);
	TIM_OC4PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);
	
	TIM_Cmd(GENERAL_TIM5,ENABLE);//使能计数器
	
}

void T5_Init(void)
{
	TIM5_GPIO_Config();
	TIM5_Config();
}

