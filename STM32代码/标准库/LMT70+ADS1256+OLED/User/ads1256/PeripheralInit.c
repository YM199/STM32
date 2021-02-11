//-----------------------------------------------------------------
// 程序描述:
// 　　芯片外设初始化
// 作　　者: 凌智电子
// 开始日期: 2014-01-24
// 完成日期: 2014-01-24
// 修改日期: 2014-02-20
// 版　　本: V1.0
// 　- V1.0: STM32F103VCT6外设初始化
// 调试工具: 凌智STM32+FPGA电子系统设计开发板、2.8寸液晶屏、
// 					 LZE_ST_LINK2
// 说　　明: 
//				
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>	
#include "PeripheralInit.h"
#include "ads1256.h"
#include "Delay.h"
// #include "usb_lib.h"
// #include "usb_desc.h"
// #include "hw_config.h"
// #include "usb_pwr.h"
//-----------------------------------------------------------------
// 功能程序区
//-----------------------------------------------------------------

/*********************************************************************************************************
*	函 数 名: ADS1256_GPIO_Init
*	功能说明: ADC GPIO初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);  			
	// 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13; 							
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	RST_L;
	Delay_1ms(1);
	RST_H;
	Delay_1ms(100);
	CS_H;
	SCLK_L;
	DIN_H;
}
/*********************************************************************************************************
*	函 数 名: PeripheralInit
*	功能说明: 系统外设初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void PeripheralInit(void)
{	
	ADS1256_GPIO_Init();						//ADC GPIO初始化
	Delay_5ms (100);
}
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
