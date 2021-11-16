#include "mykey.h"
Key_Config Key_Buf[KEY_NUM];/*创建按键数组*/
#define KEY_LONG_DOWN_DELAY 20 /*设置20个TIM3定时器中断20*50 = 1s算长按*/

/**
 * @brief  初始化按键
 * @param  Init 按键的初始化配置
 * @return NULL
*/
static void Create_Key (Key_Init* Init)
{
	for (uint8_t i = 0; i < KEY_NUM; ++i)
	{
		Key_Buf[i].Key_Board = Init[i];   /*初始化按键对象的属性*/
		Key_Buf[i].Key_Board.key_index = i; /*初始化按键的索引*/
		
		/*初始化按键的状态机*/
		Key_Buf[i].KeyStatus.KEY_SHIELD = ENABLE;//不挂起当前按键
		Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*计数器清0*/
		Key_Buf[i].KeyStatus.KEY_FLAG = NPress_Down;//按键未按下
		
		if (Key_Buf[i].Key_Board.GPIO_Pull == GPIO_PULLUP)/*按键为上拉输入*/
		{
			Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL = NPress_Down;/*按键按下时为低电平*/
		}
		else
		{
			Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL = Press_Down;/*按键按下时为高电平*/
		}
		
		Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_NULL;/*按键NEXT状态为无动作*/
		Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*事件初始化为KEY_NULL*/
		Key_Buf[i].KeyStatus.READ_PIN = KEY_ReadPin;/*赋值按键读取函数*/
	}
}

/**
 * @brief  初始化按键
 * @return NULL
*/
void KEY_Init ( void )
{
	Key_Init KeyInit[KEY_NUM] = 
	{
		{GPIO_PULLDOWN, GPIOA, GPIO_PIN_0}, /*初始化按键KEY1*/
		{GPIO_PULLDOWN, GPIOC, GPIO_PIN_13},/*初始化按键KEY2*/
	};
	Create_Key(KeyInit);/*调用按键初始化函数*/
}

/**
 * @brief  扫描按键
 * @return NULL
*/
static void Get_Key_Level (void)
{	
	for (uint8_t i = 0; i < KEY_NUM; ++i)
	{
		if (Key_Buf[i].KeyStatus.KEY_SHIELD == DISABLE)
		{
			continue;/*该按键被挂起，不操作*/
		}
		
		if (Key_Buf[i].KeyStatus.READ_PIN(Key_Buf[i].Key_Board) == Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL)
		{
			Key_Buf[i].KeyStatus.KEY_FLAG = Press_Down;/*置1，表示按键按下*/
		}
		else
		{
			Key_Buf[i].KeyStatus.KEY_FLAG = NPress_Down;/*置0，表示按键未按下*/
		}
	}
}

/**
 * @brief  状态机的状态转换
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
			case KEY_NULL:/*状态0：没有按键按下*/
				if ( Key_Buf[i].KeyStatus.KEY_FLAG == Press_Down )/*有按键按下*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_SURE;/*转入状态1*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*空事件*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*空事件*/
				}
				break;
			case KEY_SURE:/*状态1，按键按下确认*/
				if ( Key_Buf[i].KeyStatus.KEY_FLAG == Press_Down )/*确认和上次相同*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_PRESS;/*转入状态2*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_PRESS; /*按下事件*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*计数器清零，用来统计按了多久*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_NULL;/*转入状态0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*空事件*/
				}
				break;
			case KEY_PRESS:/*状态2，按键按下*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG != Press_Down)/*按键释放*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_NULL;/*转入状态0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_RAISE;/*松开事件*/
				}
				else if ((Key_Buf[i].KeyStatus.KEY_FLAG == Press_Down) && (++Key_Buf[i].KeyStatus.KEY_TIMECOUNT >= KEY_LONG_DOWN_DELAY))/*超过KEY_LONG_DOWN_DELAY没有释放*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_LONG;/*转入状态3*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_LONG;/*长按事件*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*计数器清零*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*空事件*/
				}
				break;
			case KEY_LONG:/*状态3，按键连续按下*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG != Press_Down)/*按键释放*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_NULL;/*转入状态0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_RAISE;/*松开事件*/
				}
				else if ((Key_Buf[i].KeyStatus.KEY_FLAG == Press_Down) && (++Key_Buf[i].KeyStatus.KEY_TIMECOUNT >= KEY_LONG_DOWN_DELAY))/*超过KEY_LONG_DOWN_DELAY没有释放*/
				{
					Key_Buf[i].KeyStatus.KEY_NEXT_STATUS = KEY_LONG;/*转入状态3*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_LONG;/*长按事件*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*计数器清零*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*空事件*/
				}
				break;
			default:
				break;
		}
	}
}

/**
 * @brief  根据Key_Init进行回调
 * @see    根据按键数目修改case 0~x
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
				case KEY_NULL:/*没有按下的操作*/
				{
					//printf ( "KEY_NULL\r\n" );
				}break;
				
				case KEY_SURE:/*确认按下的操作*/
				{
					printf ( "KEY_SURE\r\n" );
				}break;
				
				case KEY_RAISE:/*按键抬起操作*/
				{
					printf ( "KEY_RAISE\r\n" );
				}break;

				case KEY_PRESS:/*按键按下操作*/
				{
					printf ( "KEY_PRESS\r\n" );
				}break;

				case KEY_LONG:/*按键长按的操作*/
				{
					printf ( "KEY_LONG\r\n" );
				}break;
			}
		}break;
		
		case 1:
		{
			switch (key_config.KeyStatus.KEY_EVENT)
			{
				case KEY_NULL:/*没有按下的操作*/
				{
					//printf ( "KEY_NULL\r\n" );
				}break;
				
				case KEY_SURE:/*确认按下的操作*/
				{
					printf ( "KEY_SURE\r\n" );
				}break;
				
				case KEY_RAISE:/*按键抬起操作*/
				{
					printf ( "KEY_RAISE\r\n" );
				}break;

				case KEY_PRESS:/*按键按下操作*/
				{
					printf ( "KEY_PRESS\r\n" );
				}break;

				case KEY_LONG:/*按键长按的操作*/
				{
					printf ( "KEY_LONG\r\n" );
				}break;
			}break;			
		}		
	}
}

/**
 * @brief  置于定时器中的回调函数
 * @return NULL
*/
void Key_CallBack ( void )
{
	ReadKeyStatus();
	for ( int i = 0; i < KEY_NUM; ++i )
	{
		/*对相应的时间产生的动作*/
		Key_EventCallBack(Key_Buf[i]);
	}
}

