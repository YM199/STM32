#include "key.h"

volatile u8 Key; 
						    
//按键初始化函数
void Key_Scan_Init(void) //IO初始化
{ 
		My_GPIO_Init(KEY0_GPIO,KEY0_Pin,GPIO_FK_IN,GPIO_P_UP,GPIO_100MHz);//浮空输入 上拉 100m
}

//注意此函数有响应优先级,WK_UP>KEY1>KEY2>KEY3
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//-1，没有任何按键按下
//0，WK_UP按下 WK_UP
//1，KEY1按下
//2，KEY2按下
//3，KEY3按下 
u8 KEY_Scan(u8 mode)
{	 
	static u8 flag=0;
	static s8 key_up=1;//按键按松开标志
 
	if(Key==KEY_NO_PRES)
	{
		  if(mode)key_up=1;  //支持连按
			if(flag==0)
			{
					if(key_up&&(KEY0==0))
							flag=1;
					else if(KEY0==1)
						  key_up=1; 
			}
			else if(flag==1)
			{
				key_up=0;
				flag=0;
				if     (KEY0==0)    return KEY0_PRES;
			}
      return KEY_NO_PRES;// 无按键按下			
  }	    
 
 return Key;
}


