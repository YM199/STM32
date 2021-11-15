#include "bsp_GeneralTim.h"

// ��ʱ�����벶���û��Զ�������ṹ�嶨��
TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0,0,0,0};


static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// ���벶��ͨ�� GPIO ��ʼ��
static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);	
}



static void GENERAL_TIM_Mode_Config(void)
{
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_PERIOD;	
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);


	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = GENERAL_TIM_CHANNEL_x;// ���벶���ͨ��
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;// ���벶���źŵļ���
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(GENERAL_TIM, &TIM_ICInitStructure);
	
	// ������ºͲ����жϱ�־λ
  TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update|GENERAL_TIM_IT_CCx);	
  // �������ºͲ����ж�  
	TIM_ITConfig (GENERAL_TIM, TIM_IT_Update | GENERAL_TIM_IT_CCx, ENABLE );
	
	// ʹ�ܼ�����
	TIM_Cmd(GENERAL_TIM, ENABLE);
}

void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();		
}

/*********************************************END OF FILE**********************/
