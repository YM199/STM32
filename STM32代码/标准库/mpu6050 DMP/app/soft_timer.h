#ifndef _SOFT_TIMER_H_
#define _SOFT_TIMER_H_
#include "sys.h"

#include "delay.h"
 
#define  SOFT_TIMER_OPEN  1  //0�ر� 1����  

//*****************�ص�����*****************//

extern u8 data_data[];

void callback0(void *argv, u16 argc);
void callback1(void);
//*****************�ص�����*****************//

#define TIMER_CYCEL      10//��ʱ����С�ֱ��� ms ���1 10 100����������Ҫ1000������
#define TIMER_NUM         2//�����ʱ������

/* ��ʱ��ID */
#define Time0            0
#define Time1            1
 
typedef void callback(void *argv, u16 argc);

typedef struct SoftTimer 
{
	u8  state;               //״̬
	u8  mode;                //ģʽ
	u32 match;               //����ʱ��
	u32 period;              //��ʱ����
	callback *cb;            //�ص�����ָ��
	void *argv;              //����ָ��
	u16 argc;                //��������
}SoftTimer;

typedef enum tmrState 
{
	SOFT_TIMER_STOPPED = 0,  //ֹͣ
	SOFT_TIMER_RUNNING,      //����
	SOFT_TIMER_TIMEOUT       //��ʱ
}tmrState;

typedef enum tmrMode 
{
	Mode_One = 0,            //����ģʽ
	Mode_Cycel,              //����ģʽ
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
