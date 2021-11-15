#include "stm32f10x.h"
#include "OLED_I2C.h"
#include "./systick/bsp_SysTick.h"
#include "./usart/bsp_usart.h"
#include "dma.h"
#include "adc.h"
#include "stdio.h"
#include "string.h"
extern const unsigned char BMP1[];
extern __IO uint16_t ADC_ConvertedValue;  
int main(void)
{	

	USART_Config();
	SysTick_Init();   //≥ı ºªØ—”≥Ÿ∫Ø ˝
	DMA_Config();
	ADCx_Init();
	I2C_Configuration();//≈‰÷√CPUµƒ”≤º˛I2C
	OLED_Init();    //≥ı ºªØOLED
	OLED_CLS();
	Delay_ms(400);
	while(1)
	{	
		average(ADC_ConvertedValue);
		Delay_ms(50);
	} 
}
/*********************************************END OF FILE**********************/
//		for(i=0;i<4;i++)
//		{
//			OLED_ShowCN(22+i*16,0,i);									//≤‚ ‘œ‘ æ÷–Œƒ
//		}
//		Delay_s(2);		// 2s
//		OLED_ShowStr(0,3,(unsigned char*)"123",1);				//≤‚ ‘6*8◊÷∑˚
//		OLED_ShowStr(0,4,(unsigned char*)"456",2);				//≤‚ ‘8*16◊÷∑˚
//		Delay_s(5);		// 5s
//		();//«Â∆¡
//		OLEOLED_CLSD_OFF();//≤‚ ‘OLED–›√ﬂ
//		Delay_s(2);		// 2s
//		OLED_ON();//≤‚ ‘OLED–›√ﬂ∫ÛªΩ–—
//		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//≤‚ ‘BMPŒªÕºœ‘ æ
//		Delay_s(5);		// 5s

