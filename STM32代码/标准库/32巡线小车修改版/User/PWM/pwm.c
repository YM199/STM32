#include "pwm.h"

//TIM5 IO������
static void TIM5_GPIO_Config(void)
{
	GPIO_InitTypeDef TIM5_Struct;//GPIO�ṹ��
	RCC_APB2PeriphClockCmd(TIM5_GPIO_CLK, ENABLE);//��ʱ��
	
	
	TIM5_Struct.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	TIM5_Struct.GPIO_Pin = TIM5_CH1_Pin | TIM5_CH2_Pin | TIM5_CH3_Pin | TIM5_CH4_Pin;
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
	//PWM1ģʽ
	TIM5_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
	//�Ƚ����ʹ��
	TIM5_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	//���������ʹ��
	TIM5_OCStruct.TIM_OutputNState = TIM_OutputNState_Disable;
	//����Ƚ�ͨ���ߵ�ƽ��Ч
	TIM5_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	//�������ͨ����ƽ��������
	TIM5_OCStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// ���ͨ�����е�ƽ��������
	TIM5_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	//�������ͨ�����е�ƽ��������
	TIM5_OCStruct.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	
	// ����Ƚ�ͨ�� 1
	TIM5_OCStruct.TIM_Pulse = 0;//����ͨ��һ�ĳ�ʼռ�ձ�
	TIM_OC1Init(GENERAL_TIM5,&TIM5_OCStruct);//��ʼ��CH1ͨ��
	TIM_OC1PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
	
	//����Ƚ�ͨ��2
	TIM5_OCStruct.TIM_Pulse = 0;
	TIM_OC2Init(GENERAL_TIM5,&TIM5_OCStruct);
	TIM_OC2PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);
	
	//����Ƚ�ͨ��3
	TIM5_OCStruct.TIM_Pulse = 0;
	TIM_OC3Init(GENERAL_TIM5,&TIM5_OCStruct);
	TIM_OC3PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);
	
	//����Ƚ�ͨ��4
	TIM5_OCStruct.TIM_Pulse = 0;
	TIM_OC4Init(GENERAL_TIM5,&TIM5_OCStruct);
	TIM_OC4PreloadConfig(GENERAL_TIM5,TIM_OCPreload_Enable);
	
	TIM_Cmd(GENERAL_TIM5,ENABLE);//ʹ�ܼ�����
	
}

void T5_Init(void)
{
	TIM5_GPIO_Config();
	TIM5_Config();
}

