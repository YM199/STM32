#include "key.h"

volatile u8 Key; 
						    
//������ʼ������
void Key_Scan_Init(void) //IO��ʼ��
{ 
		My_GPIO_Init(KEY0_GPIO,KEY0_Pin,GPIO_FK_IN,GPIO_P_UP,GPIO_100MHz);//�������� ���� 100m
}

//ע��˺�������Ӧ���ȼ�,WK_UP>KEY1>KEY2>KEY3
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//-1��û���κΰ�������
//0��WK_UP���� WK_UP
//1��KEY1����
//2��KEY2����
//3��KEY3���� 
u8 KEY_Scan(u8 mode)
{	 
	static u8 flag=0;
	static s8 key_up=1;//�������ɿ���־
 
	if(Key==KEY_NO_PRES)
	{
		  if(mode)key_up=1;  //֧������
			if(flag==0)
			{
					if(key_up&&(KEY0==0))
							flag=1;
					else if(KEY0==1)
						  key_up=1; 
			}
			else if(flag==1)
			{
				key_up=0;
				flag=0;
				if     (KEY0==0)    return KEY0_PRES;
			}
      return KEY_NO_PRES;// �ް�������			
  }	    
 
 return Key;
}


