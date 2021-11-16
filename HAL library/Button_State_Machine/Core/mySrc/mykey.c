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
		Key_Buf[i].Key_Board.key_index = i; /*��ʼ������������*/
		
		/*��ʼ��������״̬��*/
		Key_Buf[i].KeyStatus.KEY_SHIELD = ENABLE;//������ǰ����
		Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*��������0*/
		Key_Buf[i].KeyStatus.KEY_FLAG = NPress_Down;//����δ����
		
		if (Key_Buf[i].Key_Board.GPIO_Pull == GPIO_PULLUP)/*����Ϊ��������*/
		{
			Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL = NPress_Down;/*��������ʱΪ�͵�ƽ*/
		}
		else
		{
			Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL = Press_Down;/*��������ʱΪ�ߵ�ƽ*/
		}
		
		Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_NULL;/*����NEXT״̬Ϊ�޶���*/
		Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*�¼���ʼ��ΪKEY_NULL*/
		Key_Buf[i].KeyStatus.READ_PIN = KEY_ReadPin;/*��ֵ������ȡ����*/
	}
}

/**
 * @brief  ��ʼ������
 * @return NULL
*/
void KEY_Init ( void )
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
			Key_Buf[i].KeyStatus.KEY_FLAG = Press_Down;/*��1����ʾ��������*/
		}
		else
		{
			Key_Buf[i].KeyStatus.KEY_FLAG = NPress_Down;/*��0����ʾ����δ����*/
		}
	}
}

/**
 * @brief  ״̬����״̬ת��
 * @return NULL
*/
static void ReadKeyStatus ( void )
{
	Get_Key_Level ();
  
	for ( uint8_t i = 0; i < KEY_NUM; ++i )
	{
		Key_Buf[i].KeyStatus.KEY_CUR_STATUS = Key_Buf[i].KeyStatus.KEY_NEXT_STATUS;
		switch ( Key_Buf[i].KeyStatus.KEY_CUR_STATUS )
		{
			case KEY_NULL:/*״̬0��û�а�������*/
				if ( Key_Buf[i].KeyStatus.KEY_FLAG == Press_Down )/*�а�������*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_SURE;/*ת��״̬1*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*���¼�*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*���¼�*/
				}
				break;
			case KEY_SURE:/*״̬1����������ȷ��*/
				if ( Key_Buf[i].KeyStatus.KEY_FLAG == Press_Down )/*ȷ�Ϻ��ϴ���ͬ*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_PRESS;/*ת��״̬2*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_PRESS; /*�����¼�*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*���������㣬����ͳ�ư��˶��*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_NULL;/*ת��״̬0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*���¼�*/
				}
				break;
			case KEY_PRESS:/*״̬2����������*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG != Press_Down)/*�����ͷ�*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_NULL;/*ת��״̬0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_RAISE;/*�ɿ��¼�*/
				}
				else if ((Key_Buf[i].KeyStatus.KEY_FLAG == Press_Down) && (++Key_Buf[i].KeyStatus.KEY_TIMECOUNT >= KEY_LONG_DOWN_DELAY))/*����KEY_LONG_DOWN_DELAYû���ͷ�*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_LONG;/*ת��״̬3*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_LONG;/*�����¼�*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*����������*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*���¼�*/
				}
				break;
			case KEY_LONG:/*״̬3��������������*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG != Press_Down)/*�����ͷ�*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_NULL;/*ת��״̬0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_RAISE;/*�ɿ��¼�*/
				}
				else if ((Key_Buf[i].KeyStatus.KEY_FLAG == Press_Down) && (++Key_Buf[i].KeyStatus.KEY_TIMECOUNT >= KEY_LONG_DOWN_DELAY))/*����KEY_LONG_DOWN_DELAYû���ͷ�*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_LONG;/*ת��״̬3*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_LONG;/*�����¼�*/
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

/**
 * @brief  ����Key_Init���лص�
 * @see    ���ݰ�����Ŀ�޸�case 0~x
 * @return NULL
*/
__weak void Key_EventCallBack ( Key_Config key_config )
{
	//printf ( "nox = %d\r\n", key_config.Key_Board.key_index );/*********nox*********/
	switch ( key_config.Key_Board.key_index )
	{
		case 0:
		{
			switch (key_config.KeyStatus.KEY_EVENT)
			{
				case KEY_NULL:/*û�а��µĲ���*/
				{
					//printf ( "KEY_NULL\r\n" );
				}break;
				
				case KEY_SURE:/*ȷ�ϰ��µĲ���*/
				{
					printf ( "KEY_SURE\r\n" );
				}break;
				
				case KEY_RAISE:/*����̧�����*/
				{
					printf ( "KEY_RAISE\r\n" );
				}break;

				case KEY_PRESS:/*�������²���*/
				{
					printf ( "KEY_PRESS\r\n" );
				}break;

				case KEY_LONG:/*���������Ĳ���*/
				{
					printf ( "KEY_LONG\r\n" );
				}break;
			}
		}break;
		
		case 1:
		{
			switch (key_config.KeyStatus.KEY_EVENT)
			{
				case KEY_NULL:/*û�а��µĲ���*/
				{
					//printf ( "KEY_NULL\r\n" );
				}break;
				
				case KEY_SURE:/*ȷ�ϰ��µĲ���*/
				{
					printf ( "KEY_SURE\r\n" );
				}break;
				
				case KEY_RAISE:/*����̧�����*/
				{
					printf ( "KEY_RAISE\r\n" );
				}break;

				case KEY_PRESS:/*�������²���*/
				{
					printf ( "KEY_PRESS\r\n" );
				}break;

				case KEY_LONG:/*���������Ĳ���*/
				{
					printf ( "KEY_LONG\r\n" );
				}break;
			}break;			
		}		
	}
}

/**
 * @brief  ���ڶ�ʱ���еĻص�����
 * @return NULL
*/
void Key_CallBack ( void )
{
	ReadKeyStatus();
	for ( int i = 0; i < KEY_NUM; ++i )
	{
		/*����Ӧ��ʱ������Ķ���*/
		Key_EventCallBack(Key_Buf[i]);
	}
}

