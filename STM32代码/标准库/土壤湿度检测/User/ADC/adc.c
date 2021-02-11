#include "adc.h"

static void ADCx_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitTypeDefStructure;
	
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitTypeDefStructure.GPIO_Pin = GPIO_Pin_1;
	
	GPIO_Init(GPIOC,&GPIO_InitTypeDefStructure);
}

static void ADC_Mode_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	ADC_InitTypeDef ADC_InitTypeDefStructure;
	
	ADC_InitTypeDefStructure.ADC_ContinuousConvMode = ENABLE;//连续转换
	ADC_InitTypeDefStructure.ADC_DataAlign =ADC_DataAlign_Right;//数据右对齐
	ADC_InitTypeDefStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitTypeDefStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitTypeDefStructure.ADC_NbrOfChannel = 1;
	ADC_InitTypeDefStructure.ADC_ScanConvMode = DISABLE;
	
	ADC_Init(ADC1,&ADC_InitTypeDefStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//8分频
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_239Cycles5);
	
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);
	
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

static void ADC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitTypeDefStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDefStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitTypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeDefStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitTypeDefStructure.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitTypeDefStructure);
}

void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADC_Mode_Config();
	ADC_NVIC_Config();
}
