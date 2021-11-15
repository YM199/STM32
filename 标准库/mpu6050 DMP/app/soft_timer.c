#include "soft_timer.h"
/*
    //中断调用
	  if(xitong_haomiao%TIMER_CYCEL==0)
			SoftTimer_Update();
*/ 
void SoftTimer_Test(void)
{
		SoftTimer_Init();
	  while(1)
		{
	
		}
}

////////////////////////////////////////移植修改区域///////////////////////////////////////////////////////////
u8 data_data[] = {1,2,3,4,5,6,7,8,9,0};

void callback0(void *argv, u16 argc)
{
		u8 i=0;
		printf("data=");
		for(i=0;i<argc;i++)
			printf("%d ",*((u8*)argv + i));
		printf("\r\n");
}

void callback1(void)
{
		printf("111\r\n");
}
//*****************回掉函数*****************//
void SoftTimer_Init(void)
{
	  SoftTimer_Start(Time0, Mode_One,   5000, callback0, data_data, 5);//适用于过一会关闭某个功能的应用
	  SoftTimer_Start(Time1, Mode_Cycel, 1000, callback1, NULL,      0);//适用于周期调用的应用
}
////////////////////////////////////////移植修改区域///////////////////////////////////////////////////////////

SoftTimer timer[TIMER_NUM];        //软件定时器数组
 
/*****************************************************
* function: 获取时钟节拍
* return:   时钟节拍 
******************************************************/
u32 TickCnt_Get(void)
{
  return xitong_haomiao;
}
 
/*****************************************************
* function: 获取软件定时器状态
* param:    软件定时器ID
* return:   定时器状态 
******************************************************/
u8 SoftTimer_GetState(u16 id)
{
	return timer[id].state;
}

/*****************************************************
* function: 启动软件定时器
* param1:   软件定时器ID
* param2:   定时器模式
* param3:   延时时间(对周期定时器，亦是周期时间)，单位 ms
* param4:   回调函数指针
* param5:   回调函数参数：指针参数
* param6:   回调函数参数：数值参数   
******************************************************/
void SoftTimer_Start(u16 id, tmrMode mode, u32 delay, callback *cb, void *argv, u16 argc)
{
	timer[id].match  = TickCnt_Get() + delay;
	timer[id].period = delay;
	timer[id].state  = SOFT_TIMER_RUNNING;
	timer[id].mode   = mode;
	timer[id].cb     = cb;
	timer[id].argv   = argv;
	timer[id].argc   = argc;
}

/*****************************************************
* function: 停止软件定时器
* param:    软件定时器ID 
******************************************************/
void SoftTimer_Stop(u16 id)
{
	timer[id].state = SOFT_TIMER_STOPPED;
}

/*****************************************************
* function: 更新软件定时器状态
主函数while循环调用，或者滴答定时器中断内调用
******************************************************/
void SoftTimer_Update(void)
{
	u16 i;
	
	for(i=0; i<TIMER_NUM; i++) {
	  switch (timer[i].state) {
      case SOFT_TIMER_STOPPED:
			  break;
		
		  case SOFT_TIMER_RUNNING:
			  if(timer[i].match <= TickCnt_Get()) {
				  timer[i].state = SOFT_TIMER_TIMEOUT;
					timer[i].cb(timer[i].argv, timer[i].argc);       //执行回调函数
			  }
			  break;
			
		  case SOFT_TIMER_TIMEOUT:
				if(timer[i].mode == Mode_One) {
			    timer[i].state = SOFT_TIMER_STOPPED;
				} else {
					timer[i].match = TickCnt_Get() + timer[i].period;
					timer[i].state = SOFT_TIMER_RUNNING;
				}
			  break;
		
		  default:
			  printf("timer[%d] state error!\r\n", i);
			  break;
	  }
  }
}
/*****************************************************
* function: 空操作
* param1:   指针参数
* param2:   数值参数
******************************************************/
void nop(void *argv, u16 argc)
{
}
