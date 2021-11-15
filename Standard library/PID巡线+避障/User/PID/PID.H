#ifndef _pid_
#define _pid_
#include "stm32f10x.h"

typedef struct
{
	float sv;//目标值
	float pv;//当前值
	
	float Ek;//本次偏差
	float kp;//比例系数
	
	
	float Ek_1;//上次偏差
	float SEk; //历史偏差之和
	float out_0;
	float out;
}PID;
extern PID pid;
void PID_Init(void);
void pid_Calc(u8 num);
#endif
