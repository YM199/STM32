#include "stm32f10x.h"
#include "OLED_I2C.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "dma.h"
#include "adc.h"
#include "stdio.h"
#include "string.h"
#include "ads1256.h"
#include "PeripheralInit.h"
extern const unsigned char BMP1[];
extern __IO uint16_t ADC_ConvertedValue;  
int main(void)
{	
	USART_Config();
	SysTick_Init();   //��ʼ���ӳٺ���
	ADS1256_GPIO_Init();
	ADS1256_CfgADC(PGA_1, DATARATE_100);	//����ADC������ ����1:1, ����������� 15Hz 
	I2C_Configuration();//����CPU��Ӳ��I2C
	OLED_Init();    //��ʼ��OLED
	OLED_CLS();
	Delay_ms(400);
	while(1)
	{	
		average(ADC_ConvertedValue);
		Delay_ms(50);
	} 
}
/*********************************************END OF FILE**********************/


