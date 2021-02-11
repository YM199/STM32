#include "pid.h"
#include "bsp_AdvanceTim.h" 
#include "usart.h"
PID pid_coor;//存放PID算法需要的数据

PID pid_dis;
void PID_Init(void)
{
	pid_coor.sv = 320;//目标值
	pid_coor.kp = -0.1;//系数
	pid_coor.out_0 = 122;


	pid_dis.sv = 10;
	pid_dis.kp = -40;//系数
	pid_dis.out_0 = 850;
}


void pid_coordinate(void)
{
	pid_coor.Ek = pid_coor.sv - pid_coor.pv;
	pid_coor.out = pid_coor.kp*pid_coor.Ek+pid_coor.out_0;
	if(pid_coor.out>170)
	{
		pid_coor.out=170;
	}
	else if(pid_coor.out<80)
	{
		pid_coor.out=80;
	}
	printf("%f",pid_coor.out);
	TIM_SetCompare1(ADVANCE_TIM,(u16)pid_coor.out);

}

void pid_distance(void)
{
	
	pid_dis.Ek = pid_dis.sv - pid_dis.pv;
	pid_dis.out = pid_dis.kp*pid_dis.Ek+pid_dis.out_0;
	
	if(pid_dis.out>2000)
	{
		pid_dis.out=2000;
	}
	else if(pid_dis.out<700)
	{
		pid_dis.out=700;
	}
	//printf("%f",pid_dis.Ek);
//	printf("out:%f\r\n",pid_dis.out);
	if(pid_dis.pv==0)
	{
		TIM_SetCompare2(ADVANCE_TIM,0);
		TIM_SetCompare3(ADVANCE_TIM,0);	
//		printf("%f\r\n",pid_dis.out);
	}
	else if(pid_dis.pv!=0)
	{
		TIM_SetCompare2(ADVANCE_TIM,(u16)pid_dis.out);
		TIM_SetCompare3(ADVANCE_TIM,(u16)pid_dis.out);
//		printf("%f\r\n",pid_dis.out);
	}
	
}

