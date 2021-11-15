#include "pid.h"
#include "bsp_AdvanceTim.h" 
#include "usart1.h"
#include "usart2.h"
PID pid;//存放PID算法需要的数据
void PID_Init(void)
{
	pid.sv = 90;//目标值
	pid.kp = 0.5;//系数
	pid.out_0 = 115;
	
}




void pid_Calc(u8 num)
{	
	pid.Ek_1 = pid.Ek;
	pid.pv = num;//当前数据
	pid.Ek = pid.sv - pid.pv;
	pid.out =  (-1.03)*pid.Ek+(0.01)*(pid.Ek-pid.Ek_1)+pid.out_0;
	if(pid.out>170)
	{
		pid.out = 170;
		TIM_SetCompare2(ADVANCE_TIM,0);//小车右后轮
		TIM_SetCompare3(ADVANCE_TIM,1150);//小车左后轮
	}
	else if(pid.out<80)
	{
		pid.out = 80;
		TIM_SetCompare2(ADVANCE_TIM,1150);//小车右后轮
		TIM_SetCompare3(ADVANCE_TIM,0);//小车左后轮
	}
	else
	{
		pid.out = pid.out;
		TIM_SetCompare2(ADVANCE_TIM,1170);//小车右后轮
		TIM_SetCompare3(ADVANCE_TIM,1170);//小车左后轮
	}
	
	
	if(pid.Ek>0)
	{
		//偏左
		TIM_SetCompare1(ADVANCE_TIM,pid.out);
	}
	else if(pid.Ek<0)
	{
		//偏右
		TIM_SetCompare1(ADVANCE_TIM,pid.out);
	}
	

//	TIM_SetCompare1(ADVANCE_TIM,pid.out);
//	Usart_SendByte(USART1,pid.out);
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	//printf("%f\r\n",pid.out);
}

