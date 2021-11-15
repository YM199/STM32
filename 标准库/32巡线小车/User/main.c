#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "track.h"
#include "pwm.h"
#include "oled.h"
#include "oled_bmp.h"
#include "oled_font.h"
int main(void)
{
	uint8_t value;
	T5_Init();
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
//	OLED_Init();
//	OLED_Draw12864BMP(BMP1);
	Track_GPIO_Config();
	while(1)
	{
	value = Track_Scan();
	//寻线模式
	switch(value)
	{
		case 1:
		
			//左车轮		
			TIM_SetCompare1(GENERAL_TIM5,0);
			TIM_SetCompare2(GENERAL_TIM5, STOP);
		
			//右车轮		
			TIM_SetCompare3(GENERAL_TIM5, 0);
			TIM_SetCompare4(GENERAL_TIM5, GO);
			break;
		case 2:
			//左车轮		
			TIM_SetCompare1(GENERAL_TIM5,0);
			TIM_SetCompare2(GENERAL_TIM5, GO);
		
			//右车轮		
			TIM_SetCompare3(GENERAL_TIM5, 0);
			TIM_SetCompare4(GENERAL_TIM5, STOP);
			break;
		case 'S':
			//左车轮		
			TIM_SetCompare1(GENERAL_TIM5,0);
			TIM_SetCompare2(GENERAL_TIM5, 0);
		
			//右车轮		
			TIM_SetCompare3(GENERAL_TIM5, 0);
			TIM_SetCompare4(GENERAL_TIM5, 0);
		break;
		case 'G':
			//左车轮
			TIM_SetCompare1(GENERAL_TIM5,0);
			TIM_SetCompare2(GENERAL_TIM5, GO);
		
			//右车轮		
			TIM_SetCompare3(GENERAL_TIM5, 0);
			TIM_SetCompare4(GENERAL_TIM5, GO);
		break;
	}
}
}


