#include "tim_tim.h"
 
void TIM2_Time_Init(u16 arr,u16 psc)
{
	  //����Ƶ��=Tclk/7200=72000000/7200=10000Hz  ��һ������ʱ1/10000s  ����5000��=5000/10000s=500ms
	  //Tout(���ʱ��)=(arr+1)*(psc+1)/Tclk =5000*7200/72000000s=500ms
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef         NVIC_InitStructure;	
 
	  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  //����ȱʡֵ,��һ����ü��Ϸ�ֹ�ŵ����ڳ�ʼ���������
	  //TIM_OCStructInit(&TIM_OCInitStructure);        //����ȱʡֵ,��һ����ü��Ϸ�ֹ�ŵ����ڳ�ʼ���������
	  //TIM_ICStructInit(&TIM_ICInitStructure);        //����ȱʡֵ,��һ����ü��Ϸ�ֹ�ŵ����ڳ�ʼ��������� 
 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
 
		TIM_TimeBaseStructure.TIM_Period = arr; //�Զ���װ�ؼĴ������ڵ�ֵ�����ֵ	
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //ʱ��Ƶ��Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:���벶��ģʽ�����˲�
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//�����ظ�������������Ƕ��ٴ����������жϣ�һ��Ϊ0��ֻ�и߼���ʱ��������
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //��������ж�
 
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure); 
		
    TIM_SetCounter(TIM2, 0);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM_Cmd(TIM2, ENABLE);  
}
 