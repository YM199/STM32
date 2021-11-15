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


//static void Get_Key_Level (void)
//{	
//	for (uint8_t i = 0; i < KEY_NUM; ++i)
//	{
//		if (Key_Buf[i].KeyStatus.KEY_SHIELD == DISABLE)
//		{
//			continue;/*该按键被挂起，不操作*/
//		}
//		
//		if (Key_Buf[i].KeyStatus.READ_PIN(Key_Buf[i].Key_Board) == )
//	}
//}