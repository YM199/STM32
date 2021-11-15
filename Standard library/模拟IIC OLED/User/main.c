/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "oled.h"
#include "iic.h"
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  USART_Config();
	IIC_Init();
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
  while(1)
	{
		Usart_RecData();		

		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
	}
}
/*********************************************END OF FILE**********************/
