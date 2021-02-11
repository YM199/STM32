//-----------------------------------------------------------------
// ��������:
// ����оƬ�����ʼ��
// ��������: ���ǵ���
// ��ʼ����: 2014-01-24
// �������: 2014-01-24
// �޸�����: 2014-02-20
// �桡����: V1.0
// ��- V1.0: STM32F103VCT6�����ʼ��
// ���Թ���: ����STM32+FPGA����ϵͳ��ƿ����塢2.8��Һ������
// 					 LZE_ST_LINK2
// ˵������: 
//				
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
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
// ���ܳ�����
//-----------------------------------------------------------------

/*********************************************************************************************************
*	�� �� ��: ADS1256_GPIO_Init
*	����˵��: ADC GPIO��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: PeripheralInit
*	����˵��: ϵͳ�����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************/
void PeripheralInit(void)
{	
	ADS1256_GPIO_Init();						//ADC GPIO��ʼ��
	Delay_5ms (100);
}
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
