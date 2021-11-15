#include "bsp_encoder.h"

void Encoder_Init()
{
	//GPIO��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//�������� ����������
	GPIO_InitStructure.GPIO_Pin =  Encoder_CH1|Encoder_CH2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Encoder_GPIO,&GPIO_InitStructure);
	
	//��ʱ����ʼ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_DeInit(Encoder_Tim);//��TIM�ļĴ���ֵ�ָ�����λ״̬
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = Parameter * 4;//�������ֱ��� * 4
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(Encoder_Tim,&TIM_TimeBaseStructure);
	//ģʽ3 ��������
	TIM_EncoderInterfaceConfig(Encoder_Tim,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//�����˲���
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(Encoder_Tim,&TIM_ICInitStructure);
	
	//��������ֵ��ֵΪ0
	Encoder_Tim->CNT = 0;
	//����������
	TIM_Cmd(Encoder_Tim,ENABLE);
}
