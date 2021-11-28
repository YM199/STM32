#ifndef __XPT2046_LCD_H
#define	__XPT2046_LCD_H

#include "main.h"
#include "bsp_ili9341_lcd.h"



//触屏信号有效电平
#define   XPT2046_PENIRQ_ActiveLevel       0
#define   XPT2046_PENIRQ_Read()        HAL_GPIO_ReadPin ( XPT2046_PENIRQ_GPIO_Port, XPT2046_PENIRQ_Pin )



/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}	//设置为高电平		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}//输出低电平

#define   XPT2046_CS_ENABLE()      digitalHi( XPT2046_SPI_CS_GPIO_Port, XPT2046_SPI_CS_Pin )    
#define   XPT2046_CS_DISABLE()     digitalLo ( XPT2046_SPI_CS_GPIO_Port, XPT2046_SPI_CS_Pin )  

#define   XPT2046_CLK_HIGH()       digitalHi ( XPT2046_SPI_CLK_GPIO_Port, XPT2046_SPI_CLK_Pin )    
#define   XPT2046_CLK_LOW()        digitalLo ( XPT2046_SPI_CLK_GPIO_Port, XPT2046_SPI_CLK_Pin ) 

#define   XPT2046_MOSI_1()         digitalHi ( XPT2046_SPI_MOSI_GPIO_Port, XPT2046_SPI_MOSI_Pin ) 
#define   XPT2046_MOSI_0()         digitalLo ( XPT2046_SPI_MOSI_GPIO_Port, XPT2046_SPI_MOSI_Pin )

#define   XPT2046_MISO()           HAL_GPIO_ReadPin ( XPT2046_SPI_MISO_GPIO_Port, XPT2046_SPI_MISO_Pin )

/******************************* XPT2046 触摸屏参数定义 ***************************/
//校准触摸屏时触摸坐标的AD值相差门限 
#define             XPT2046_THRESHOLD_CalDiff                    2               

#define	            XPT2046_CHANNEL_X 	                          0x90 	          //通道Y+的选择控制字	
#define	            XPT2046_CHANNEL_Y 	                          0xd0	          //通道X+的选择控制字



/******************************* 声明 XPT2046 相关的数据类型 ***************************/
typedef	struct          //液晶坐标结构体 
{		
	/*负数值表示无新数据*/
   int16_t x;			//记录最新的触摸参数值
   int16_t y; 
	
	/*用于记录连续触摸时(长按)的上一次触摸位置*/
	 int16_t pre_x;		
   int16_t pre_y;
	
} strType_XPT2046_Coordinate;   





typedef struct         //校准系数结构体（最终使用）
{
	 float dX_X,  			 
					dX_Y,     
					dX,   
					dY_X,    
					dY_Y,    
					dY;

} strType_XPT2046_TouchPara;

/******触摸状态机相关******/
typedef enum
{
	XPT2046_STATE_RELEASE  = 0,	//触摸释放
	XPT2046_STATE_WAITING,			//触摸按下
	XPT2046_STATE_PRESSED,			//触摸按下
}enumTouchState	;

#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			0

//触摸消抖阈值
#define DURIATION_TIME				2

uint8_t XPT2046_TouchDetect(void);
uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara );
void XPT2046_TouchEvenHandler(void );
#endif /*__XPT2046_LCD_H*/

