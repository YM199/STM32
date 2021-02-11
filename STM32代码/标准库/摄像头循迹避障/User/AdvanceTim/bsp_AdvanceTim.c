#include "bsp_AdvanceTim.h" 

static void NVIT_TIM_Config()
{
	NVIC_InitTypeDef NVIC_InitStructrue;
	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* 配置定时器1溢出为中断源 */
	NVIC_InitStructrue.NVIC_IRQChannel = GENERAL_TIM_IRQ;
	/* 使能中断 */
	NVIC_InitStructrue.NVIC_IRQChannelCmd = ENABLE;
	/* 抢断优先级*/
	NVIC_InitStructrue.NVIC_IRQChannelPreemptionPriority = 0;
	/* 使能中断 */
	NVIC_InitStructrue.NVIC_IRQChannelSubPriority = 3;
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructrue);
}

/************TIM5控制减速电机***************/

//TIM5 IO口配置
static void TIM5_GPIO_Config(void)
{
	GPIO_InitTypeDef TIM5_Struct;//GPIO结构体
	RCC_APB2PeriphClockCmd(TIM5_GPIO_CLK, ENABLE);//打开时钟
	
	
	TIM5_Struct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	TIM5_Struct.GPIO_Pin = TIM5_CH1_Pin | TIM5_CH2_Pin;
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

	TIM5_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM1模式
	TIM5_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出比较通道高电平有效
	TIM5_OCStruct.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	
	// 输出比较通道 1
	TIM5_OCStruct.TIM_Pulse = GERENAL_TIM5_PULSE;//设置通道一的初始占空比
	TIM_OC1Init(GENERAL_TIM5,&TIM5_OCStruct);//初始化CH1通道
	TIM_OC1PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);//使能预装载寄存器
	
	//输出比较通道2
	TIM5_OCStruct.TIM_Pulse = GERENAL_TIM5_PULSE;
	TIM_OC2Init(GENERAL_TIM5,&TIM5_OCStruct);
	TIM_OC2PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);
		
	/*****************************中断配置****************************/
	//清除计数器中断标志位
	TIM_ClearFlag(GENERAL_TIM,TIM_FLAG_Update);
	//开启定时器中断
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);	
		
	TIM_Cmd(GENERAL_TIM5,ENABLE);//使能计数器
	
}


/************高级定时器控制舵机***************/
static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输出比较通道1 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);
	
	// 输出比较通道2 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH2_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH2_PORT, &GPIO_InitStructure);

  // 输出比较通道刹车通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_BKIN_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_BKIN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStructure);
	// BKIN引脚默认先输出低电平
	GPIO_ResetBits(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN);	
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void ADVANCE_TIM_Mode_Config(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
	/*--------------------输出比较结构体初始化-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//互补输出不使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//互补输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	//互补输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	
	// 设置通道1占空比大小
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	
	// 设置通道4占空比大小
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	TIM_OC4Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);

	/*-------------------刹车和死区结构体初始化-------------------*/
	// 有关刹车和死区结构体的成员具体可参考BDTR寄存器的描述
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	// 输出比较信号死区时间配置，具体如何计算可参考 BDTR:UTG[7:0]的描述
	// 这里配置的死区时间为3us
  TIM_BDTRInitStructure.TIM_DeadTime = 216;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// 当BKIN引脚检测到高电平的时候，输出比较信号被禁止，就好像是刹车一样
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	// 使能计数器
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_GPIO_Config();
	ADVANCE_TIM_Mode_Config();
	NVIT_TIM_Config();
}


void T5_Init(void)
{
	TIM5_GPIO_Config();
	TIM5_Config();
}
/*********************************************END OF FILE**********************/
