#include "stm32f10x.h"
#include "bsp_usart.h"
#include "dma.h"
#include "bsp_adc.h"
// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue;
// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal;  
// 软件延时
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 
int main(void)
{
	USART_Config();
	DMA_Config();
	ADCx_Init();
	while (1)
	{
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // 读取转换的AD值
		printf("\r\n 电压 = %f V \r\n",ADC_ConvertedValueLocal); 
		Delay(0xffffee);  
	}
}


