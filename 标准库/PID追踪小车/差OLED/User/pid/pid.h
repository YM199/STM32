#ifndef _PID_H
#define _PID_H
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
void PID_Init(void);
void pid_coordinate(void);
void pid_distance(void);
#endif

