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

/**
 * @brief  ��ʼ������
 * @return NULL
*/
void KEY_Init(void)
{
	Key_Init KeyInit[KEY_NUM] = 
	{
		{GPIO_PULLDOWN, GPIOA, GPIO_PIN_0}, /*��ʼ������KEY1*/
		{GPIO_PULLDOWN, GPIOC, GPIO_PIN_13},/*��ʼ������KEY2*/
	};
	Create_Key(KeyInit);/*���ð�����ʼ������*/
}

/**
 * @brief  ɨ�谴��
 * @return NULL
*/
static void Get_Key_Level (void)
{	
	for (uint8_t i = 0; i < KEY_NUM; ++i)
	{
		if (Key_Buf[i].KeyStatus.KEY_SHIELD == DISABLE)
		{
			continue;/*�ð��������𣬲�����*/
		}
		
		if (Key_Buf[i].KeyStatus.READ_PIN(Key_Buf[i].Key_Board) == Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL)
		{
			Key_Buf[i].KeyStatus.KEY_FLAG = HIGH_LEVEL;/*��1����ʾ��������*/
		}
		else
		{
			Key_Buf[i].KeyStatus.KEY_FLAG = LOW_LEVEL;/*��0����ʾ����δ����*/
		}
	}
}

/**
 * @brief  ״̬����״̬ת��
 * @return NULL
*/
static void ReadKeyStatus(void)
{
	Get_Key_Level();
	for (uint8_t i = 0; i < KEY_NUM; ++i)
	{
		switch (Key_Buf[i].KeyStatus.KEY_STATUS)
		{
			case KEY_NULL:/*״̬0��û�а�������*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG == HIGH_LEVEL)/*�а�������*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_SURE;/*ת��״̬1*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*���¼�*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*���¼�*/
				}
				break;
			case KEY_SURE:/*״̬1����������ȷ��*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG == HIGH_LEVEL)/*ȷ�Ϻ��ϴ���ͬ*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_PRESS;/*ת��״̬2*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_PRESS; /*�����¼�*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*���������㣬����ͳ�ư��˶��*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_NULL;/*ת��״̬0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*���¼�*/
				}
				break;
			case KEY_PRESS:/*״̬2����������*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG != HIGH_LEVEL)/*�����ͷ�*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_NULL;/*ת��״̬0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_RAISE;/*�ɿ��¼�*/
				}
				else if ((Key_Buf[i].KeyStatus.KEY_FLAG == HIGH_LEVEL) && (++Key_Buf[i].KeyStatus.KEY_TIMECOUNT >= KEY_LONG_DOWN_DELAY))/*����KEY_LONG_DOWN_DELAYû���ͷ�*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_LONG;/*ת��״̬3*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_LONG;/*�����¼�*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*����������*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*���¼�*/
				}
				break;
			case KEY_LONG:/*״̬3��������������*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG != HIGH_LEVEL)/*�����ͷ�*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_NULL;/*ת��״̬0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_RAISE;/*�ɿ��¼�*/
				}
				else if ((Key_Buf[i].KeyStatus.KEY_FLAG == HIGH_LEVEL) && (++Key_Buf[i].KeyStatus.KEY_TIMECOUNT >= KEY_LONG_DOWN_DELAY))/*����KEY_LONG_DOWN_DELAYû���ͷ�*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_LONG;/*ת��״̬3*/
					//Key_Buf[i].KeyStatus.KEY_EVENT = KEY_LONG;/*�����¼�*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*����������*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*���¼�*/
				}
				break;
			default:
				break;
		}
	}
}

