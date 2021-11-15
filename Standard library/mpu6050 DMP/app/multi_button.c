#include "multi_button.h"
#include "usart1.h"	  
#include "delay.h"	 

/*
    //中断调用
	  if(xitong_haomiao%TICKS_INTERVAL==0)
			button_ticks();
*/

void Multi_Button_Test(void)
{
	Multi_Button_Init(); 
  u32 i=0;
  while(1) 
	{
#if Multi_Button_Callback ==0 || Multi_Button_Callback >1
				if(Button1_Event != Get_Button_Event(&Button1)) 
				{
						Button1_Event = Get_Button_Event(&Button1);

						if(Button1_Event == PRESS_DOWN) 
						{
								printf("PRESS_DOWN\r\n");
						} 
						else if(Button1_Event == PRESS_UP) 
						{
								printf("PRESS_UP\r\n");
						} 
						else if(Button1_Event == PRESS_REPEAT) 
						{
								printf("PRESS_REPEAT\r\n");
						}
						else if(Button1_Event == SINGLE_CLICK) 
						{
								printf("SINGLE_CLICK\r\n");
						}
						else if(Button1_Event == DOUBLE_CLICK) 
						{
								printf("DOUBLE_CLICK\r\n");
						}
						else if(Button1_Event == LONG_RRESS_START) 
						{
								printf("LONG_RRESS_START\r\n");
						}
						else if(Button1_Event == LONG_PRESS_HOLD) 
						{
							  printf("LONG_PRESS_HOLD\r\n");
							  while(Get_Button_Event(&Button1)==LONG_PRESS_HOLD)
								{
										delay_ms(100);
										i++;
										USART1_printf("i=%d\r\n",i);
								}
						}
	      }
#endif
	 }
}

////////////////////////////////////////移植修改区域///////////////////////////////////////////////////////////
#define Button1_GPIO  GPIOA
#define Button1_Pin   GPIO_Pin_8
struct  Button        Button1;
        u8            Button1_Event;
 
#if Multi_Button_Callback >0
void BTN1_PRESS_DOWN_Handler(void* btn)
{
	printf("PRESS_DOWN_Callback\r\n");
}
void BTN1_PRESS_UP_Handler(void* btn)
{
	printf("PRESS_UP_Callback\r\n");
}
void BTN1_PRESS_REPEAT_Handler(void* btn)
{
	printf("PRESS_REPEAT_Callback=%d\r\n",Button1.repeat);
}
void BTN1_SINGLE_Click_Handler(void* btn)
{
	printf("SINGLE_CLICK_Callback\r\n");
}
void BTN1_DOUBLE_Click_Handler(void* btn)
{
	printf("DOUBLE_CLICK_Callback\r\n");
}
void BTN1_LONG_RRESS_START_Handler(void* btn)
{
	printf("LONG_RRESS_START_Callback\r\n");
}
void BTN1_LONG_PRESS_HOLD_Handler(void* btn)
{
	printf("LONG_PRESS_HOLD_Callback\r\n");
}
#endif

//按键读取函数
u8 Read_Button1_GPIO(void){return GPIO_ReadInputDataBit(Button1_GPIO,Button1_Pin);}

void Multi_Button_Init(void)
{
	//按键引脚初始化
  My_GPIO_Init (Button1_GPIO,Button1_Pin,GPIO_FK_IN,GPIO_P_UP,GPIO_50MHz);//浮空输入 上拉 50m
	//定义按键按下电平
  Button_Init  (&Button1, Read_Button1_GPIO, 0);//按下时低电平
	
#if Multi_Button_Callback ==0
	Button_Attach(&Button1, PRESS_DOWN,        0);
	Button_Attach(&Button1, PRESS_UP,          0);
	Button_Attach(&Button1, PRESS_REPEAT,      0);
	Button_Attach(&Button1, SINGLE_CLICK,      0);
	Button_Attach(&Button1, DOUBLE_CLICK,      0);
	Button_Attach(&Button1, LONG_RRESS_START,  0);
	Button_Attach(&Button1, LONG_PRESS_HOLD,   0);
#else
	Button_Attach(&Button1, PRESS_DOWN,        BTN1_PRESS_DOWN_Handler);
	Button_Attach(&Button1, PRESS_UP,          BTN1_PRESS_UP_Handler);
	Button_Attach(&Button1, PRESS_REPEAT,      BTN1_PRESS_REPEAT_Handler);
	Button_Attach(&Button1, SINGLE_CLICK,      BTN1_SINGLE_Click_Handler);
	Button_Attach(&Button1, DOUBLE_CLICK,      BTN1_DOUBLE_Click_Handler);
	Button_Attach(&Button1, LONG_RRESS_START,  BTN1_LONG_RRESS_START_Handler);
	Button_Attach(&Button1, LONG_PRESS_HOLD,   BTN1_LONG_PRESS_HOLD_Handler);
#endif
}
////////////////////////////////////////移植修改区域///////////////////////////////////////////////////////////

#define EVENT_CB(ev)   if(handle->cb[ev])handle->cb[ev]((Button*)handle)
	
//button handle list head.
static struct Button* head_handle = NULL;

/**
  * @brief  Initializes the button struct handle.
  * @param  handle: the button handle strcut.
  * @param  pin_level: read the HAL GPIO of the connet button level.
  * @param  active_level: pressed GPIO level.
  * @retval None
  */
void Button_Init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level)
{
	memset(handle, 0, sizeof(struct Button));
	handle->event = (uint8_t)NONE_PRESS;
	handle->hal_button_Level = pin_level;
	handle->button_level = handle->hal_button_Level();
	handle->active_level = active_level;
	
	Button_Start(handle);
}

/**
  * @brief  Attach the button event callback function.
  * @param  handle: the button handle strcut.
  * @param  event: trigger event type.
  * @param  cb: callback function.
  * @retval None
  */
void Button_Attach(struct Button* handle, PressEvent event, BtnCallback cb)
{
	handle->cb[event] = cb;
}

/**
  * @brief  Inquire the button event happen.
  * @param  handle: the button handle strcut.
  * @retval button event.
  */
PressEvent Get_Button_Event(struct Button* handle)
{
	return (PressEvent)(handle->event);
}

/**
  * @brief  Button driver core function, driver state machine.
  * @param  handle: the button handle strcut.
  * @retval None
  */
void Button_Handler(struct Button* handle)
{
	uint8_t read_gpio_level = handle->hal_button_Level();

	//ticks counter working..
	if((handle->state) > 0) handle->ticks++;

	/*------------button debounce handle---------------*/
	if(read_gpio_level != handle->button_level) { //not equal to prev one
		//continue read 3 times same new level change
		if(++(handle->debounce_cnt) >= DEBOUNCE_TICKS) {
			handle->button_level = read_gpio_level;
			handle->debounce_cnt = 0;
		}
	} else { //leved not change ,counter reset.
		handle->debounce_cnt = 0;
	}

	/*-----------------State machine-------------------*/
	switch (handle->state) {
	case 0:
		if(handle->button_level == handle->active_level) {	//start press down
			handle->event = (uint8_t)PRESS_DOWN;
			EVENT_CB(PRESS_DOWN);
			handle->ticks = 0;
			handle->repeat = 1;
			handle->state = 1;
		} else {
			handle->event = (uint8_t)NONE_PRESS;
		}
		break;

	case 1:
		if(handle->button_level != handle->active_level) { //released press up
			handle->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			handle->ticks = 0;
			handle->state = 2;

		} else if(handle->ticks > LONG_TICKS) {
			handle->event = (uint8_t)LONG_RRESS_START;
			EVENT_CB(LONG_RRESS_START);
			handle->state = 5;
		}
		break;

	case 2:
		if(handle->button_level == handle->active_level) { //press down again
			handle->event = (uint8_t)PRESS_DOWN;
			EVENT_CB(PRESS_DOWN);
			handle->repeat++;
			if(handle->repeat == 2) {
				EVENT_CB(DOUBLE_CLICK); // repeat hit
			} 
			EVENT_CB(PRESS_REPEAT); // repeat hit
			handle->ticks = 0;
			handle->state = 3;
		} else if(handle->ticks > SHORT_TICKS) { //released timeout
			if(handle->repeat == 1) {
				handle->event = (uint8_t)SINGLE_CLICK;
				EVENT_CB(SINGLE_CLICK);
			} else if(handle->repeat == 2) {
				handle->event = (uint8_t)DOUBLE_CLICK;
			}
			handle->state = 0;
		}
		break;

	case 3:
		if(handle->button_level != handle->active_level) { //released press up
			handle->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			if(handle->ticks < SHORT_TICKS) {
				handle->ticks = 0;
				handle->state = 2; //repeat press
			} else {
				handle->state = 0;
			}
		}
		break;

	case 5:
		if(handle->button_level == handle->active_level) {
			//continue hold trigger
			handle->event = (uint8_t)LONG_PRESS_HOLD;
			EVENT_CB(LONG_PRESS_HOLD);

		} else { //releasd
			handle->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			handle->state = 0; //reset
		}
		break;
	}
}

/**
  * @brief  Start the button work, add the handle into work list.
  * @param  handle: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int Button_Start(struct Button* handle)
{
	struct Button* target = head_handle;
	while(target) {
		if(target == handle) return -1;	//already exist.
		target = target->next;
	}
	handle->next = head_handle;
	head_handle = handle;
	return 0;
}

/**
  * @brief  Stop the button work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
void Button_Stop(struct Button* handle)
{
	struct Button** curr;
	for(curr = &head_handle; *curr; ) {
		struct Button* entry = *curr;
		if (entry == handle) {
			*curr = entry->next;
		} else
			curr = &entry->next;
	}
}

/**
  * @brief  background ticks, timer repeat invoking interval 5ms.
  * @param  None.
  * @retval None
  */

//周期调用这个函数 if(xitong_haomiao%TICKS_INTERVAL==0)button_ticks();
void Button_Ticks(void)
{
	struct Button* target;
	for(target=head_handle; target; target=target->next) {
		Button_Handler(target);
	}
}

