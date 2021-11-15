#include "hc_sr04.h"   


void Trig_GPIO_Config(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( Trig_GPIO_PORT_GPIO_CLK , ENABLE);
		GPIO_InitStructure.GPIO_Pin = Trig_GPIO_PORT_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(Trig_GPIO_PORT, &GPIO_InitStructure);	
}

void Start(void)
{
	GPIO_SetBits(Trig_GPIO_PORT,Trig_GPIO_PORT_GPIO_PIN);
	SysTick_Delay_Us(20);
	GPIO_ResetBits(Trig_GPIO_PORT,Trig_GPIO_PORT_GPIO_PIN);
}

void calculation_distance(void)
{
	uint32_t time;
	double distance;
	if(TIM_ICUserValueStructure.Capture_FinishFlag == 1)
	{
	// 计算高电平时间的计数器的值
	time = TIM_ICUserValueStructure.Capture_Period * (GENERAL_TIM_PERIOD+1) + 
			       (TIM_ICUserValueStructure.Capture_CcrValue+1);
	printf ( "时间：%dus\r\n",time);
	distance = (time/2)*34/1000;
	printf ( "距离：%0.3fcm\r\n",distance);
	TIM_ICUserValueStructure.Capture_FinishFlag = 0;	
	}
}

