#ifndef _MULTI_BUTTON_H_
#define _MULTI_BUTTON_H_

#include "sys.h"

// https://github.com/0x1abin/MultiButton 
 
#define Multi_Button_OPEN      1//0关闭 1开启  

#define Multi_Button_Callback  1//0:仅开启查询方案  1:仅开启回调函数  2:同时开启

extern struct Button Button1;
extern        u8     Button1_Event;

 
//According to your need to modify the constants.
#define TICKS_INTERVAL     5	//ms  //周期调用这个函数 if(xitong_haomiao%TICKS_INTERVAL==0)button_ticks();
#define DEBOUNCE_TICKS     3	//MAX 7 越大允许两次间停顿时间越大
#define SHORT_TICKS       (300 /TICKS_INTERVAL)
#define LONG_TICKS        (1000 /TICKS_INTERVAL)


typedef void (*BtnCallback)(void*);

typedef enum {
	PRESS_DOWN = 0,
	PRESS_UP,
	PRESS_REPEAT,    //repeat 最大15
	SINGLE_CLICK,
	DOUBLE_CLICK,
	LONG_RRESS_START,
	LONG_PRESS_HOLD, //以上7状态可用 
	number_of_event,
	NONE_PRESS
}PressEvent;

typedef struct Button {
	u16 ticks;
	u8  repeat : 4;      //最大15
	u8  event  : 4;
	u8  state  : 3;
	u8  debounce_cnt : 3;//最大7
	u8  active_level : 1;
	u8  button_level : 1;
	u8  (*hal_button_Level)(void);
	BtnCallback  cb[number_of_event];
	struct Button* next;
}Button;

#ifdef __cplusplus  
extern "C" {  
#endif 

void       Multi_Button_Test(void);
void       Multi_Button_Init(void);
void       Button_Init(struct Button* handle, u8(*pin_level)(), u8 active_level);
void       Button_Attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent Get_Button_Event(struct Button* handle);
int        Button_Start(struct Button* handle);
void       Button_Stop(struct Button* handle);
void       Button_Ticks(void);

#ifdef __cplusplus
} 
#endif

#endif
