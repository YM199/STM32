#include "tim_tim.h"
 
void TIM2_Time_Init(u16 arr,u16 psc)
{
	  //计数频率=Tclk/7200=72000000/7200=10000Hz  计一个数用时1/10000s  计数5000次=5000/10000s=500ms
	  //Tout(溢出时间)=(arr+1)*(psc+1)/Tclk =5000*7200/72000000s=500ms
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef         NVIC_InitStructure;	
 
	  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  //设置缺省值,这一步最好加上防止放到串口初始化后出问题
	  //TIM_OCStructInit(&TIM_OCInitStructure);        //设置缺省值,这一步最好加上防止放到串口初始化后出问题
	  //TIM_ICStructInit(&TIM_ICInitStructure);        //设置缺省值,这一步最好加上防止放到串口初始化后出问题 
 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
 
		TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载寄存器周期的值，溢出值	
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //时钟频率预分频值
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:输入捕获模式用来滤波
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//设置重复溢出次数，就是多少次溢出后进入中断，一般为0，只有高级定时器才有用
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //允许更新中断
 
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure); 
		
    TIM_SetCounter(TIM2, 0);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM_Cmd(TIM2, ENABLE);  
}
 