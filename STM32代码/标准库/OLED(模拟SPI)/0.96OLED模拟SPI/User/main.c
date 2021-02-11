#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "spi.h"
#include "bsp_usart.h"
#include "oled.h"
/*
 * t : ��ʱʱ�� 
 * Ticks : ���ٸ�ʱ�����ڲ���һ���ж� 
 * f : ʱ��Ƶ�� 72000000
 * t = Ticks * 1/f = (72000000/100000) * (1/72000000) = 10us 
 */ 

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	USART_Config();
	Software_SPI_Init();
	OLED_Init();
	while(1)
	{
		OLED_ShowCHinese(0,0,4);
		OLED_8x16Str(2,2,"2020/11/10");
		OLED_8x16Str(2,4,"CSDN");
	}
}












/*********************************************END OF FILE**********************/
