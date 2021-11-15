#include "mykey.h"
Key_Config Key_Buf[KEY_NUM];/*������������*/
#define KEY_LONG_DOWN_DELAY 20 /*����20��TIM3��ʱ���ж�20*50 = 1s�㳤��*/

/**
 * @brief  ��ʼ������
 * @param  Init �����ĳ�ʼ������
 * @return NULL
*/
static void Create_Key (Key_Init* Init)
{
	for (uint8_t i = 0; i < KEY_NUM; ++i)
	{
		Key_Buf[i].Key_Board = Init[i];   /*��ʼ���������������*/
		Key_Buf[i].Key_Board.key_nox = i; /*��ʼ���������������ǵø��±�����*/
		
		/*��ʼ��������״̬��*/
		Key_Buf[i].KeyStatus.KEY_SHIELD = ENABLE;//������ǰ����
		Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*---------------------*/
		Key_Buf[i].KeyStatus.KEY_FLAG = LOW_LEVEL;//����δ����
		
		if (Key_Buf[i].Key_Board.GPIO_Pull == GPIO_PULLUP)/*����Ϊ��������*/
		{
			Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL = LOW_LEVEL;/*��������ʱΪ�͵�ƽ*/
		}
		else
		{
			Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL = HIGH_LEVEL;/*��������ʱΪ�ߵ�ƽ*/
		}
		
		Key_Buf[i].KeyStatus.KEY_STATUS = KEY_NULL;/*����״̬Ϊ�޶���*/
		Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*---------------------*/
		Key_Buf[i].KeyStatus.READ_PIN = KEY_ReadPin;/*��ֵ������ȡ����*/
	}
}


//static void Get_Key_Level (void)
//{	
//	for (uint8_t i = 0; i < KEY_NUM; ++i)
//	{
//		if (Key_Buf[i].KeyStatus.KEY_SHIELD == DISABLE)
//		{
//			continue;/*�ð��������𣬲�����*/
//		}
//		
//		if (Key_Buf[i].KeyStatus.READ_PIN(Key_Buf[i].Key_Board) == )
//	}
//}