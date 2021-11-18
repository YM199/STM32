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

2021/11/18修复bug

**原因**：提前调用`HAL_TIM_Base_Start_IT(&htim1);` 打开了定时器，导致进入中断的时候任务还没开始初始化，传入`vTaskNotifyGiveFromISR`的任务句柄为空，进入断言函数导致大于等于BASEPRI的中断被屏蔽（包括了定时器中断）。

**解决办法**：在任务初始化之后打开定时器：

```C
  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  HAL_TIM_Base_Start_IT(&htim1);
  /* Start scheduler */
```

**缺点**：在不合适的地方添加了自定义代码，如果使用STM32Cubemx重新生成了代码，`HAL_TIM_Base_Start_IT(&htim1);`需要重新添加到指定位置。
