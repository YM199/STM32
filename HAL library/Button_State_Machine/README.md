**修改工程需要修改的地方：**

设置按键的属性

```c
void KEY_Init ( void )
{
	Key_Init KeyInit[KEY_NUM] = 
	{
		{GPIO_PULLDOWN, GPIOA, GPIO_PIN_0}, /*初始化按键KEY0*/
		{GPIO_PULLDOWN, GPIOC, GPIO_PIN_13},/*初始化按键KEY1*/
	};
	Create_Key(KeyInit);/*调用按键初始化函数*/
}
```



有几个按键就有几个case

```c
__weak void Key_EventCallBack ( Key_Config key_config )
{
	//printf ( "nox = %d\r\n", key_config.Key_Board.key_index );/*********nox*********/
	switch ( key_config.Key_Board.key_index )
	{
		case 0:
		{

		}break;
		
		case 1:
		{
			
		}break;		
	}
}
```

有几个按键就有几个KEY

```C
/*按键注册表*/
typedef enum
{
	KEY0,
	KEY1,/*用户添加的按钮名称*/
	KEY_NUM,/*必须要有的记录按钮数量，必须在最后*/
}KEY_LIST;
```





# BUG

无法在定时器中断里面发送任务通知，一发送就死机，按键中断可以。
