#include "bsp_AdvanceTim.h" 

static void NVIT_TIM_Config()
{
	NVIC_InitTypeDef NVIC_InitStructrue;
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* ���ö�ʱ��1���Ϊ�ж�Դ */
	NVIC_InitStructrue.NVIC_IRQChannel = GENERAL_TIM_IRQ;
	/* ʹ���ж� */
	NVIC_InitStructrue.NVIC_IRQChannelCmd = ENABLE;
	/* �������ȼ�*/
	NVIC_InitStructrue.NVIC_IRQChannelPreemptionPriority = 0;
	/* ʹ���ж� */
	NVIC_InitStructrue.NVIC_IRQChannelSubPriority = 3;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructrue);
}

/************TIM5���Ƽ��ٵ��***************/

//TIM5 IO������
static void TIM5_GPIO_Config(void)
{
	GPIO_InitTypeDef TIM5_Struct;//GPIO�ṹ��
	RCC_APB2PeriphClockCmd(TIM5_GPIO_CLK, ENABLE);//��ʱ��
	
	
	TIM5_Struct.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	TIM5_Struct.GPIO_Pin = TIM5_CH1_Pin | TIM5_CH2_Pin;
	TIM5_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(TIM5_GPIO,&TIM5_Struct);	
}

static void TIM5_Config()
{
	//������ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	//��ʼ��ʱ���ṹ���ʼ��
	TIM_TimeBaseInitTypeDef TIM5_TimeBaseStruct;
	TIM5_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ 1��Ƶ
	TIM5_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;//���¶���
	TIM5_TimeBaseStruct.TIM_Period = GENERAL_TIM5_Period;
	TIM5_TimeBaseStruct.TIM_Prescaler = GENERAL_TIM5_PSC;
	TIM5_TimeBaseStruct.TIM_RepetitionCounter = 0;//�ظ������� ֻ�����ڸ߼���ʱ�� 
	TIM_TimeBaseInit(GENERAL_TIM5, &TIM5_TimeBaseStruct);
	
	//��ʱ������ȽϽṹ���ʼ��
	TIM_OCInitTypeDef TIM5_OCStruct;

	TIM5_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM1ģʽ
	TIM5_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;//����Ƚ�ͨ���ߵ�ƽ��Ч
	TIM5_OCStruct.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	
	// ����Ƚ�ͨ�� 1
	TIM5_OCStruct.TIM_Pulse = GERENAL_TIM5_PULSE;//����ͨ��һ�ĳ�ʼռ�ձ�
	TIM_OC1Init(GENERAL_TIM5,&TIM5_OCStruct);//��ʼ��CH1ͨ��
	TIM_OC1PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
	
	//����Ƚ�ͨ��2
	TIM5_OCStruct.TIM_Pulse = GERENAL_TIM5_PULSE;
	TIM_OC2Init(GENERAL_TIM5,&TIM5_OCStruct);
	TIM_OC2PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);
		
	/*****************************�ж�����****************************/
	//����������жϱ�־λ
	TIM_ClearFlag(GENERAL_TIM,TIM_FLAG_Update);
	//������ʱ���ж�
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);	
		
	TIM_Cmd(GENERAL_TIM5,ENABLE);//ʹ�ܼ�����
	
}


/************�߼���ʱ�����ƶ��***************/
static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ����Ƚ�ͨ��1 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);
	
	// ����Ƚ�ͨ��2 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH2_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH2_PORT, &GPIO_InitStructure);

  // ����Ƚ�ͨ��ɲ��ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_BKIN_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_BKIN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStructure);
	// BKIN����Ĭ��������͵�ƽ
	GPIO_ResetBits(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN);	
}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void ADVANCE_TIM_Mode_Config(void)
{
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// ����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//���������ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	// ���ͨ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//�������ͨ����ƽ��������
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// ���ͨ�����е�ƽ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	//�������ͨ�����е�ƽ��������
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	
	// ����ͨ��1ռ�ձȴ�С
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	
	// ����ͨ��4ռ�ձȴ�С
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	TIM_OC4Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);

	/*-------------------ɲ���������ṹ���ʼ��-------------------*/
	// �й�ɲ���������ṹ��ĳ�Ա����ɲο�BDTR�Ĵ���������
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	// ����Ƚ��ź�����ʱ�����ã�������μ���ɲο� BDTR:UTG[7:0]������
	// �������õ�����ʱ��Ϊ3us
  TIM_BDTRInitStructure.TIM_DeadTime = 216;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// ��BKIN���ż�⵽�ߵ�ƽ��ʱ������Ƚ��źű���ֹ���ͺ�����ɲ��һ��
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	// ʹ�ܼ�����
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
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
