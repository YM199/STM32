#ifndef _KEY_H_
#define _KEY_H_	 
#include "sys.h"

volatile extern u8 Key;

#define Key_Scan_OPEN        1//0�ر� 1����  
#define Key_Scan_Time       20
 
#define KEY0_GPIO   GPIOA
#define KEY0_Pin    GPIO_Pin_8
#define KEY0        PAin(8)   //PA8 

#define KEY_NO_PRES 	    0	
#define KEY0_PRES	        1	
 
void Key_Scan_Init(void);    //IO��ʼ��
u8   KEY_Scan(u8);  	       //����ɨ�躯��

#endif
