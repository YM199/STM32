#ifndef _PID_H
#define _PID_H
#include "stm32f10x.h"

typedef struct
{
	float sv;//Ŀ��ֵ
	float pv;//��ǰֵ
	
	float Ek;//����ƫ��
	float kp;//����ϵ��
	
	
	float Ek_1;//�ϴ�ƫ��
	float SEk; //��ʷƫ��֮��
	float out_0;
	float out;
}PID;
void PID_Init(void);
void pid_coordinate(void);
void pid_distance(void);
#endif

