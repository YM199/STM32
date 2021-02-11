#include "track.h"

//����ģ������ҷֱ��Ӧ13,12

void Track_GPIO_Config(void)
{
	GPIO_InitTypeDef Track_structer;//��ʼ���ṹ��
	RCC_APB2PeriphClockCmd(TRACK_GPIO_CLOCK,ENABLE);//��GPIOB��ʱ��
	
	Track_structer.GPIO_Mode = GPIO_Mode_IPD;//��������
	Track_structer.GPIO_Pin = Track1 | Track2;
	
	GPIO_Init(Track_GPIO,&Track_structer);//GPIO��ʼ��	
}

//��߽�13,�ұ߽�12
uint8_t Track_Scan(void)
{
	uint16_t Track_Value;
	Track_Value = (GPIO_ReadInputData(Track_GPIO)&0X3000);//ȡ16λ���ݵ�13,12λ
	
	if(Track_Value == 0X2000)//13��⵽����  ��ת
	{
		return 1;
	}
	
	else if(Track_Value == 0X1000)//12��⵽���� ��
	{
		return 2;
	}
	else if(Track_Value == 0X3000)//13,12��⵽����.ͣ��
	{
		return 'S';
	}
	
	else 
		return 'G';//1,2����ģ�鶼û��⵽���� ֱ��
}

