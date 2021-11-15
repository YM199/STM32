#include "stm32f10x.h"
#include "OLED_I2C.h"
#include "./systick/bsp_SysTick.h"
#include "./usart/bsp_usart.h"

extern const unsigned char BMP1[];

int main(void)
{
	unsigned char i;
	USART_Config();
	SysTick_Init();   //��ʼ���ӳٺ���
	I2C_Configuration();//����CPU��Ӳ��I2C
	OLED_Init();    //��ʼ��OLED
	while(1)
	{
		OLED_Fill(0xFF);//ȫ������
		Delay_s(2);		// 2s
		OLED_Fill(0x00);//ȫ����
		Delay_s(2);		// 2s
		
		for(i=0;i<4;i++)
		{
			OLED_ShowCN(22+i*16,0,i);									//������ʾ����
		}
		Delay_s(2);		// 2s
		OLED_ShowStr(0,3,(unsigned char*)"123",1);				//����6*8�ַ�
		OLED_ShowStr(0,4,(unsigned char*)"456",2);				//����8*16�ַ�
		Delay_s(5);		// 5s
		OLED_CLS();//����
		OLED_OFF();//����OLED����
		Delay_s(2);		// 2s
		OLED_ON();//����OLED���ߺ���
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
		Delay_s(5);		// 5s
	} 
}
/*********************************************END OF FILE**********************/
