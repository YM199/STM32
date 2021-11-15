#ifndef _SOFT_TIMER_H_
#define _SOFT_TIMER_H_
#include "sys.h"

#include "delay.h"
 
#define  SOFT_TIMER_OPEN  1  //0关闭 1开启  

//*****************回掉函数*****************//

extern u8 data_data[];

void callback0(void *argv, u16 argc);
void callback1(void);
//*****************回掉函数*****************//

#define TIMER_CYCEL      10//定时器最小分辨率 ms 最好1 10 100这三个，不要1000及以上
#define TIMER_NUM         2//软件定时器数量

/* 定时器ID */
#define Time0            0
#define Time1            1
 
typedef void callback(void *argv, u16 argc);

typedef struct SoftTimer 
{
	u8  state;               //状态
	u8  mode;                //模式
	u32 match;               //到期时间
	u32 period;              //定时周期
	callback *cb;            //回调函数指针
	void *argv;              //参数指针
	u16 argc;                //参数个数
}SoftTimer;

typedef enum tmrState 
{
	SOFT_TIMER_STOPPED = 0,  //停止
	SOFT_TIMER_RUNNING,      //运行
	SOFT_TIMER_TIMEOUT       //超时
}tmrState;

typedef enum tmrMode 
{
	Mode_One = 0,            //单次模式
	Mode_Cycel,              //周期模式
}tmrMode;

void SoftTimer_Test(void);
void SoftTimer_Init(void);
void SoftTimer_Start(u16 id, tmrMode mode, u32 delay, callback *cb, void *argv, u16 argc);
void SoftTimer_Stop(u16 id);
void SoftTimer_Update(void);

u32  TickCnt_Get(void);
u8   SoftTimer_GetState(u16 id);
void nop(void *argv, u16 argc);
#endif 
