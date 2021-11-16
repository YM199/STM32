#ifndef __MYKEY_H__
#define __MYKEY_H__

#include "main.h"
#include "tim.h"
/* 用于状态机初始化按键 */
typedef struct{
	uint32_t GPIO_Pull;		/*按键的上下拉模式*/
	GPIO_TypeDef* GPIOx;	/*按键对应的端口*/
	uint16_t GPIO_Pin_x;	/*按键的引脚*/
	uint8_t key_index;    /*按键的索引*/
}Key_Init;

/* 按键状态机的五种状态 */
typedef enum {
	KEY_NULL = 0u,/* 无动作*/
	KEY_SURE,      /* 确认状态*/
	KEY_RAISE,     /* 按键抬起*/
	KEY_PRESS,     /* 按键按下*/
	KEY_LONG       /* 长按*/
}KEY_STATUS_LIST;

/*按键屏蔽标志*/
typedef FunctionalState KEY_ENABLE_STATUS;

/*按键IO读取标志*/
#define NPress_Down  GPIO_PIN_RESET/*按键未按下*/
#define Press_Down   GPIO_PIN_SET  /*按键按下*/
typedef GPIO_PinState IO_STATUS_LIST;

/*读取IO电平的函数 函数指针  内敛函数?*/
static IO_STATUS_LIST KEY_ReadPin(Key_Init Key)
{
	return (IO_STATUS_LIST)HAL_GPIO_ReadPin(Key.GPIOx, Key.GPIO_Pin_x);
}

/*状态机类*/
typedef struct __KEY_COMPONENTS
{
	KEY_ENABLE_STATUS KEY_SHIELD;      /*按键屏蔽：DISABLE(0);屏蔽，ENABLE(1);不屏蔽*/
	uint8_t KEY_TIMECOUNT;             /*按键长按计数*/
	IO_STATUS_LIST KEY_FLAG;           /*是否按下的标志 1表示按下*/
	IO_STATUS_LIST KEY_DOWN_LEVEL;     /*按下时，按键IO实际的电平*/
	KEY_STATUS_LIST KEY_CUR_STATUS;    /*按键CUR状态*/
	KEY_STATUS_LIST KEY_NEXT_STATUS;   /*按键NEXT状态*/
	KEY_STATUS_LIST KEY_EVENT;         /*按键事件*/
	IO_STATUS_LIST (*READ_PIN) (Key_Init Key);/*读IO电平函数，这是一个函数指针*/
}KEY_COMPONENTS;


/*按键类,代表了一个按键*/
typedef struct
{
	Key_Init Key_Board;      /*继承初始化父类*/
	KEY_COMPONENTS KeyStatus;/*继承状态机父类*/
}Key_Config;

/*按键注册表*/
typedef enum
{
	KEY0,
	KEY1,/*用户添加的按钮名称*/
	KEY_NUM,/*必须要有的记录按钮数量，必须在最后*/
}KEY_LIST;

void KEY_Init ( void );
void Key_CallBack ( void );
#endif  /*__MYKEY_H__*/

