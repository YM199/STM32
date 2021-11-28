#ifndef __XPT2046_LCD_H
#define	__XPT2046_LCD_H

#include "main.h"
#include "bsp_ili9341_lcd.h"



//�����ź���Ч��ƽ
#define   XPT2046_PENIRQ_ActiveLevel       0
#define   XPT2046_PENIRQ_Read()        HAL_GPIO_ReadPin ( XPT2046_PENIRQ_GPIO_Port, XPT2046_PENIRQ_Pin )



/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRR=i;}	//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}//����͵�ƽ

#define   XPT2046_CS_ENABLE()      digitalHi( XPT2046_SPI_CS_GPIO_Port, XPT2046_SPI_CS_Pin )    
#define   XPT2046_CS_DISABLE()     digitalLo ( XPT2046_SPI_CS_GPIO_Port, XPT2046_SPI_CS_Pin )  

#define   XPT2046_CLK_HIGH()       digitalHi ( XPT2046_SPI_CLK_GPIO_Port, XPT2046_SPI_CLK_Pin )    
#define   XPT2046_CLK_LOW()        digitalLo ( XPT2046_SPI_CLK_GPIO_Port, XPT2046_SPI_CLK_Pin ) 

#define   XPT2046_MOSI_1()         digitalHi ( XPT2046_SPI_MOSI_GPIO_Port, XPT2046_SPI_MOSI_Pin ) 
#define   XPT2046_MOSI_0()         digitalLo ( XPT2046_SPI_MOSI_GPIO_Port, XPT2046_SPI_MOSI_Pin )

#define   XPT2046_MISO()           HAL_GPIO_ReadPin ( XPT2046_SPI_MISO_GPIO_Port, XPT2046_SPI_MISO_Pin )

/******************************* XPT2046 �������������� ***************************/
//У׼������ʱ���������ADֵ������� 
#define             XPT2046_THRESHOLD_CalDiff                    2               

#define	            XPT2046_CHANNEL_X 	                          0x90 	          //ͨ��Y+��ѡ�������	
#define	            XPT2046_CHANNEL_Y 	                          0xd0	          //ͨ��X+��ѡ�������



/******************************* ���� XPT2046 ��ص��������� ***************************/
typedef	struct          //Һ������ṹ�� 
{		
	/*����ֵ��ʾ��������*/
   int16_t x;			//��¼���µĴ�������ֵ
   int16_t y; 
	
	/*���ڼ�¼��������ʱ(����)����һ�δ���λ��*/
	 int16_t pre_x;		
   int16_t pre_y;
	
} strType_XPT2046_Coordinate;   





typedef struct         //У׼ϵ���ṹ�壨����ʹ�ã�
{
	 float dX_X,  			 
					dX_Y,     
					dX,   
					dY_X,    
					dY_Y,    
					dY;

} strType_XPT2046_TouchPara;

/******����״̬�����******/
typedef enum
{
	XPT2046_STATE_RELEASE  = 0,	//�����ͷ�
	XPT2046_STATE_WAITING,			//��������
	XPT2046_STATE_PRESSED,			//��������
}enumTouchState	;

#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			0

//����������ֵ
#define DURIATION_TIME				2

uint8_t XPT2046_TouchDetect(void);
uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara );
void XPT2046_TouchEvenHandler(void );
#endif /*__XPT2046_LCD_H*/

