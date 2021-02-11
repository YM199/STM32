#include "track.h"

//红外模块从左到右分别对应13,12

void Track_GPIO_Config(void)
{
	GPIO_InitTypeDef Track_structer;//初始化结构体
	RCC_APB2PeriphClockCmd(TRACK_GPIO_CLOCK,ENABLE);//打开GPIOB的时钟
	
	Track_structer.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
	Track_structer.GPIO_Pin = Track1 | Track2;
	
	GPIO_Init(Track_GPIO,&Track_structer);//GPIO初始化	
}

//左边接13,右边接12
uint8_t Track_Scan(void)
{
	uint16_t Track_Value;
	Track_Value = (GPIO_ReadInputData(Track_GPIO)&0X3000);//取16位数据的13,12位
	
	if(Track_Value == 0X2000)//13检测到黑线  左转
	{
		return 1;
	}
	
	else if(Track_Value == 0X1000)//12检测到黑线 右
	{
		return 2;
	}
	else if(Track_Value == 0X3000)//13,12检测到黑线.停车
	{
		return 'S';
	}
	
	else 
		return 'G';//1,2红外模块都没检测到黑线 直走
}

