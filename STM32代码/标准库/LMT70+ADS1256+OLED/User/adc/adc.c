#include "adc.h"
#include "./systick/bsp_SysTick.h"
#include "./usart/bsp_usart.h"
#include "OLED_I2C.h"
#include "stdio.h"
#include "string.h"
#include "ads1256.h"
// ADC1转换的电压值通过MDA方式传到变量
extern __IO uint16_t ADC_ConvertedValue;
// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal;
	double receive,temp;
	u8 buf[50];
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOC, ENABLE );
	
	// 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// 初始化 ADC IO
	GPIO_Init(GPIOC, &GPIO_InitStructure);				
}

/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */
static void ADCx_Mode_Config(void)
{

	ADC_InitTypeDef ADC_InitStructure;
	
	// 打开ADC时钟
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );

	// ADC 模式配置
	// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// 连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道1个
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// 初始化ADC
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 配置ADC时钟为PCLK2的6分频，即12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	// 配置 ADC 通道转换顺序为1，第一个转换，采样时间为55.5个时钟周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_41Cycles5);
	
	// 使能ADC DMA 请求
	ADC_DMACmd(ADC1, ENABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(ADC1, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC1);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	// ADC开始校准
	ADC_StartCalibration(ADC1);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}



void average(u16 ADC_ConvertedValue)
{
	uint32_t adc;									//采样结果
	uint32_t adc_1;								//
	float volt;									//实际电压值

	adc = (int32_t)ADS1256_GetAdc( (7 << 4) | 0x08 );		// 读取采样的结果
	adc_1 = (adc ^ 0x800000);         //将补码转换成原码
	volt = (((0.596047*adc_1)-5000000)/1000);    //得到实际电压
//	printf("%4.3f\r\n",volt);
//	volt/=1000;
	temp=(-1.809628)/1000000000*volt*volt*volt+(-3.325395)/1000000*volt*volt+(-1.814103)/10*volt+2.055894*100;
//	printf("%f\r\n",temp);
	sprintf((char *)buf,"%.3f",temp);
	OLED_ShowStr(0,0,buf,2);
	
}
/*********************************************END OF FILE**********************/
