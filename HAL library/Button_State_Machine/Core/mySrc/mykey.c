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
		Key_Buf[i].Key_Board.key_nox = i; /*初始化按键的索引，记得改下变量名*/
		
		/*初始化按键的状态机*/
		Key_Buf[i].KeyStatus.KEY_SHIELD = ENABLE;//不挂起当前按键
		Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*---------------------*/
		Key_Buf[i].KeyStatus.KEY_FLAG = LOW_LEVEL;//按键未按下
		
		if (Key_Buf[i].Key_Board.GPIO_Pull == GPIO_PULLUP)/*按键为上拉输入*/
		{
			Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL = LOW_LEVEL;/*按键按下时为低电平*/
		}
		else
		{
			Key_Buf[i].KeyStatus.KEY_DOWN_LEVEL = HIGH_LEVEL;/*按键按下时为高电平*/
		}
		
		Key_Buf[i].KeyStatus.KEY_STATUS = KEY_NULL;/*按键状态为无动作*/
		Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*---------------------*/
		Key_Buf[i].KeyStatus.READ_PIN = KEY_ReadPin;/*赋值按键读取函数*/
	}
}

/**
 * @brief  初始化按键
 * @return NULL
*/
void KEY_Init(void)
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
			Key_Buf[i].KeyStatus.KEY_FLAG = HIGH_LEVEL;/*置1，表示按键按下*/
		}
		else
		{
			Key_Buf[i].KeyStatus.KEY_FLAG = LOW_LEVEL;/*置0，表示按键未按下*/
		}
	}
}

/**
 * @brief  状态机的状态转换
 * @return NULL
*/
static void ReadKeyStatus(void)
{
	Get_Key_Level();
	for (uint8_t i = 0; i < KEY_NUM; ++i)
	{
		switch (Key_Buf[i].KeyStatus.KEY_STATUS)
		{
			case KEY_NULL:/*状态0：没有按键按下*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG == HIGH_LEVEL)/*有按键按下*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_SURE;/*转入状态1*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*空事件*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*空事件*/
				}
				break;
			case KEY_SURE:/*状态1，按键按下确认*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG == HIGH_LEVEL)/*确认和上次相同*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_PRESS;/*转入状态2*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_PRESS; /*按下事件*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*计数器清零，用来统计按了多久*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_NULL;/*转入状态0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*空事件*/
				}
				break;
			case KEY_PRESS:/*状态2，按键按下*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG != HIGH_LEVEL)/*按键释放*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_NULL;/*转入状态0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_RAISE;/*松开事件*/
				}
				else if ((Key_Buf[i].KeyStatus.KEY_FLAG == HIGH_LEVEL) && (++Key_Buf[i].KeyStatus.KEY_TIMECOUNT >= KEY_LONG_DOWN_DELAY))/*超过KEY_LONG_DOWN_DELAY没有释放*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_LONG;/*转入状态3*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_LONG;/*长按事件*/
					Key_Buf[i].KeyStatus.KEY_TIMECOUNT = 0;/*计数器清零*/
				}
				else
				{
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_NULL;/*空事件*/
				}
				break;
			case KEY_LONG:/*状态3，按键连续按下*/
				if (Key_Buf[i].KeyStatus.KEY_FLAG != HIGH_LEVEL)/*按键释放*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_NULL;/*转入状态0*/
					Key_Buf[i].KeyStatus.KEY_EVENT = KEY_RAISE;/*松开事件*/
				}
				else if ((Key_Buf[i].KeyStatus.KEY_FLAG == HIGH_LEVEL) && (++Key_Buf[i].KeyStatus.KEY_TIMECOUNT >= KEY_LONG_DOWN_DELAY))/*超过KEY_LONG_DOWN_DELAY没有释放*/
				{
					Key_Buf[i].KeyStatus.KEY_STATUS = KEY_LONG;/*转入状态3*/
					//Key_Buf[i].KeyStatus.KEY_EVENT = KEY_LONG;/*长按事件*/
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

