 /*************************************
*		        ��è�ƴ�                    *
*		 ����:һֻ��ѧϰ��cat                *
**************************************/
#ifndef _OLED_H_
#define _OLED_H_

#include "stm32f10x_gpio.h"
#include "oled_bmp.h"
#include "oled_font.h"



/****************�ɸ����������õ�ѡ��******************/
#define DISPLAY_MODE     2     //��ʾģʽ��1Ϊ���ְ׵ף�2Ϊ���ֺڵ�

/****************************************************/


/********************���Ŷ���*************************/
//����ʵ������޸Ķ�ӦIO������
//IIC�ӿ�����SCL��SDA
//IIC_SCL 
#define SCL_GPIOClock    RCC_APB2Periph_GPIOB
#define SCL_GPIO         GPIOB
#define SCL_GPIO_Pin     GPIO_Pin_5
// IIC_SDA 
#define SDA_GPIOClock    RCC_APB2Periph_GPIOB
#define SDA_GPIO         GPIOB
#define SDA_GPIO_Pin     GPIO_Pin_6



   
#define OLED_CMD  0
#define OLED_DATA 1


extern u8 string[16];

/* OLED��ʼ������ */
void OLED_Init(void);

/* ���ܺ��� */    
void OLED_Clear(void); //����
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size);          //��ʾһ���ַ�����x���꣬y���꣬��ʾ���ַ������壩ps������Ϊ16����8
void OLED_ShowNum(u8 x,u8 y,long num,u8 len,u8 size);  //��ʾ���֣�(x���꣬y���꣬��ʾ�����֣����ȣ�����) ps������Ϊ16����8
void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);   //��ʾͼƬ������ʼx��y���꣬��ֹx��y���꣬ͼƬ��ַ��
void OLED_Draw12864BMP(u8 BMP[]);                      //��ʾ128*64��ͼƬ��ͼƬ��ַ��
void OLED_ShowString(u8 x,u8 y,char *chr,u8 size);       //��ʾ���ֺ��ַ��������������ѵǼǵĺ��֣���x���꣬y���꣬���ִ����ַ������壩
																		 //���ֵ�����̶�16*16�����ɸı�,��Ҫ���뺺�֣��������λ16������Ϊ8Ϊ���ַ���
																		 
/* PS������ʵ��sprintf����ʵ�ִ��������ַ���
������
	   ����һ��u8 string[16];�����ı���
		 Ȼ��ʹ��sprintf(string,"display_num:%d",num);
		 ��Ȼ�����OLED_ShowString(0,0,string,8);����������ʾ���������ַ�����sprintf�����ĸ����÷����԰ٶȲ���
 */

/***************************************************/

/* ��������������Ҫ���� */
void OLED_Set_Pos(u8 x,u8 y);
void OLED_WR_Byte(u8 dat,u8 cmd);


#endif


