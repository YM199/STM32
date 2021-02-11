#include "adc.h"
#include "./systick/bsp_SysTick.h"
#include "./usart/bsp_usart.h"
#include "OLED_I2C.h"
#include "stdio.h"
#include "string.h"
#include "ads1256.h"
// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ��������
extern __IO uint16_t ADC_ConvertedValue;
// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal;
	double receive,temp;
	u8 buf[50];
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOC, ENABLE );
	
	// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// ��ʼ�� ADC IO
	GPIO_Init(GPIOC, &GPIO_InitStructure);				
}

/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADCx_Mode_Config(void)
{

	ADC_InitTypeDef ADC_InitStructure;
	
	// ��ADCʱ��
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );

	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ��1��
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// ��ʼ��ADC
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// ����ADCʱ��ΪPCLK2��6��Ƶ����12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	// ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ55.5��ʱ������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_41Cycles5);
	
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADC1, ENABLE);
	
	// ����ADC ������ʼת��
	ADC_Cmd(ADC1, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC1);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC1);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC1));
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}



void average(u16 ADC_ConvertedValue)
{
	uint32_t adc;									//�������
	uint32_t adc_1;								//
	float volt;									//ʵ�ʵ�ѹֵ

	adc = (int32_t)ADS1256_GetAdc( (7 << 4) | 0x08 );		// ��ȡ�����Ľ��
	adc_1 = (adc ^ 0x800000);         //������ת����ԭ��
	volt = (((0.596047*adc_1)-5000000)/1000);    //�õ�ʵ�ʵ�ѹ
//	printf("%4.3f\r\n",volt);
//	volt/=1000;
	temp=(-1.809628)/1000000000*volt*volt*volt+(-3.325395)/1000000*volt*volt+(-1.814103)/10*volt+2.055894*100;
//	printf("%f\r\n",temp);
	sprintf((char *)buf,"%.3f",temp);
	OLED_ShowStr(0,0,buf,2);
	
}
/*********************************************END OF FILE**********************/
