 /*************************************
*		        黑猫科创                    *
*		 作者:一只爱学习的cat                *
**************************************/
#ifndef _OLED_H_
#define _OLED_H_

#include "stm32f10x_gpio.h"
#include "oled_bmp.h"
#include "oled_font.h"



/****************可根据需求设置的选项******************/
#define DISPLAY_MODE     2     //显示模式，1为黑字白底，2为白字黑底

/****************************************************/


/********************引脚定义*************************/
//根据实际情况修改对应IO口配置
//IIC接口配置SCL和SDA
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

/* OLED初始化函数 */
void OLED_Init(void);

/* 功能函数 */    
void OLED_Clear(void); //清屏
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size);          //显示一个字符，（x坐标，y坐标，显示的字符，字体）ps：字体为16或者8
void OLED_ShowNum(u8 x,u8 y,long num,u8 len,u8 size);  //显示数字，(x坐标，y坐标，显示的数字，长度，字体) ps：字体为16或者8
void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);   //显示图片，（起始x，y坐标，终止x，y坐标，图片地址）
void OLED_Draw12864BMP(u8 BMP[]);                      //显示128*64的图片（图片地址）
void OLED_ShowString(u8 x,u8 y,char *chr,u8 size);       //显示汉字和字符串，可以输入已登记的汉字，（x坐标，y坐标，文字串，字符串字体）
																		 //汉字的字体固定16*16，不可改变,若要输入汉字，字体必须位16，字体为8为纯字符串
																		 
/* PS：可以实验sprintf函数实现带参数的字符串
方法：
	   定义一个u8 string[16];这样的变量
		 然后使用sprintf(string,"display_num:%d",num);
		 再然后调用OLED_ShowString(0,0,string,8);函数即可显示带参数的字符串，sprintf函数的更多用法可以百度查找
 */

/***************************************************/

/* 其他函数，不需要调用 */
void OLED_Set_Pos(u8 x,u8 y);
void OLED_WR_Byte(u8 dat,u8 cmd);


#endif


