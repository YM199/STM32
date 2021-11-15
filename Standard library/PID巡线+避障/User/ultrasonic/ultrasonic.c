#include "ultrasonic.h"
#include "bsp_SysTick.h"
#include "usart1.h"
#include "bsp_motor.h"   
u32 temp=0; 
float UltrasonicWave_Distance=0;      //计算出的距离
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器5!


//定时器5通道1输入捕获配置



void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 下拉
	
	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
   	TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
}

/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;					 //PC8接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //初始化外设GPIO 

}

/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_StartMeasure(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	SysTick_Delay_Us(20);		                      //延时20US
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
}


float rang(void)
{
		UltrasonicWave_StartMeasure();//开始测距
 		if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=65536;//溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			
			UltrasonicWave_Distance=(float)temp*170/10000;//距离计算公式：距离=高电平持续时间*声速/2   
														  //即 距离= 高电平持续时间*340*100/（2*1000 000） = 高电平持续时间*170/10000
			if(UltrasonicWave_Distance<35)
			{
				Motor_turn();
			}
			TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
		}
		
		return UltrasonicWave_Distance;
}

void UltrasonicWave_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 
	UltrasonicWave_Configuration();
 	TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
}

