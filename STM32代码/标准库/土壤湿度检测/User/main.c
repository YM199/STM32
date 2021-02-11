#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "usart1.h"
#include "usart2.h"
#include "HC05.h"
#include "bsp_SysTick.h"
#include "oled.h"
#include "adc.h"
#include "control.h"
/*
   USART2����ָ���������USART2������Ӧ��Ȼ����Ӧͨ��USART1���͸�����
*/
extern u8 flag;//�ϵ�Ĭ�Ϲر�
u8 num = 69;
u8 buzzer1=0,buzzer2=0;
float ADC,humidity;
char cStr[30] = {0};
extern __IO uint16_t ADC_Value;

// �����ʱ
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 


int main(void)
{
	control_Init();
	USART1_Init();
	USART2_Init();
	HC05_Init();
	OLED_Init();
	OLED_Clear();
	ADCx_Init();
	while(1)
	{
		ADC = (float)ADC_Value/4096*3.3;
		humidity = 100-(100/3*ADC);
		if(humidity<0)
			humidity = 0;
		if(humidity>100)
			humidity = 100;
//		sprintf(cStr,"humidity %f",humidity);
		if(flag==1)
		{
			OLED_Clear();
			OLED_ShowString(0,0,"����ʪ��",16);
			sprintf(cStr,"%f%%",humidity);
			Usart2_SendString(USART2,cStr);
			OLED_ShowString(0,10,cStr,16);
			if(humidity<30)//ȱˮ
			{
				GPIO_SetBits(CONTROL_GPIO,CONTROL_Pin);
				if(buzzer1==0)
				{	
					buzzer1++;
					buzzer2 = 0;
					BUZZER_DIDI();
				}
			}
			if(humidity>30)
			{
				GPIO_ResetBits(CONTROL_GPIO,CONTROL_Pin);
				if(buzzer2==0)
				{	
					buzzer2++;
					buzzer1 = 0;
					BUZZER_DIDI();
				}
			}
		}
		else if(flag==0)
		{
			OLED_Clear();
			OLED_ShowString(0,0,"NO WORK",16);
			Usart2_SendString(USART2,"OFF");
			GPIO_ResetBits(CONTROL_GPIO,CONTROL_Pin); 
		}
		Delay(0xffffE4);
	}
}


