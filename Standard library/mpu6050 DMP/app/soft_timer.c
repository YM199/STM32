#include "soft_timer.h"
/*
    //�жϵ���
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

////////////////////////////////////////��ֲ�޸�����///////////////////////////////////////////////////////////
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
//*****************�ص�����*****************//
void SoftTimer_Init(void)
{
	  SoftTimer_Start(Time0, Mode_One,   5000, callback0, data_data, 5);//�����ڹ�һ��ر�ĳ�����ܵ�Ӧ��
	  SoftTimer_Start(Time1, Mode_Cycel, 1000, callback1, NULL,      0);//���������ڵ��õ�Ӧ��
}
////////////////////////////////////////��ֲ�޸�����///////////////////////////////////////////////////////////

SoftTimer timer[TIMER_NUM];        //�����ʱ������
 
/*****************************************************
* function: ��ȡʱ�ӽ���
* return:   ʱ�ӽ��� 
******************************************************/
u32 TickCnt_Get(void)
{
  return xitong_haomiao;
}
 
/*****************************************************
* function: ��ȡ�����ʱ��״̬
* param:    �����ʱ��ID
* return:   ��ʱ��״̬ 
******************************************************/
u8 SoftTimer_GetState(u16 id)
{
	return timer[id].state;
}

/*****************************************************
* function: ���������ʱ��
* param1:   �����ʱ��ID
* param2:   ��ʱ��ģʽ
* param3:   ��ʱʱ��(�����ڶ�ʱ������������ʱ��)����λ ms
* param4:   �ص�����ָ��
* param5:   �ص�����������ָ�����
* param6:   �ص�������������ֵ����   
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
* function: ֹͣ�����ʱ��
* param:    �����ʱ��ID 
******************************************************/
void SoftTimer_Stop(u16 id)
{
	timer[id].state = SOFT_TIMER_STOPPED;
}

/*****************************************************
* function: ���������ʱ��״̬
������whileѭ�����ã����ߵδ�ʱ���ж��ڵ���
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
					timer[i].cb(timer[i].argv, timer[i].argc);       //ִ�лص�����
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
* function: �ղ���
* param1:   ָ�����
* param2:   ��ֵ����
******************************************************/
void nop(void *argv, u16 argc)
{
}
