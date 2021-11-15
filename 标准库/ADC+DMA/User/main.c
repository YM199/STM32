#include "stm32f10x.h"
#include "bsp_usart.h"
#include "dma.h"
#include "bsp_adc.h"
// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;
// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal;  
// �����ʱ
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
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // ��ȡת����ADֵ
		printf("\r\n ��ѹ = %f V \r\n",ADC_ConvertedValueLocal); 
		Delay(0xffffee);  
	}
}


