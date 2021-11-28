#include "xpt2046_lcd.h"

//strType_XPT2046_TouchPara strXPT2046_TouchPara[] = { 	
// -0.006464,   -0.073259,  280.358032,    0.074878,    0.002052,   -6.545977,//扫描方式0
//	0.086314,    0.001891,  -12.836658,   -0.003722,   -0.065799,  254.715714,//扫描方式1
//	0.002782,    0.061522,  -11.595689,    0.083393,    0.005159,  -15.650089,//扫描方式2
//	0.089743,   -0.000289,  -20.612209,   -0.001374,    0.064451,  -16.054003,//扫描方式3
//	0.000767,   -0.068258,  250.891769,   -0.085559,   -0.000195,  334.747650,//扫描方式4
// -0.084744,    0.000047,  323.163147,   -0.002109,   -0.066371,  260.985809,//扫描方式5
//  0.002013,    0.065823,  -13.364482,   -0.096078,   -0.005051,  360.354095,//扫描方式6
// -0.085470,   -0.000876,  334.023163,   -0.003390,    0.064725,   -6.211169,//扫描方式7
//};






/******************************* 定义 XPT2046 全局变量 ***************************/
//默认触摸参数，不同的屏幕稍有差异，可重新调用触摸校准函数获取
//strType_XPT2046_TouchPara strXPT2046_TouchPara[] = { 	
// -0.006464,   -0.073259,  280.358032,    0.074878,    0.002052,   -6.545977,//扫描方式0
//	0.086314,    0.001891,  -12.836658,   -0.003722,   -0.065799,  254.715714,//扫描方式1
//	0.002782,    0.061522,  -11.595689,    0.083393,    0.005159,  -15.650089,//扫描方式2
//	0.089743,   -0.000289,  -20.612209,   -0.001374,    0.064451,  -16.054003,//扫描方式3
//	0.000767,   -0.068258,  250.891769,   -0.085559,   -0.000195,  334.747650,//扫描方式4
// -0.084744,    0.000047,  323.163147,   -0.002109,   -0.066371,  260.985809,//扫描方式5
//  0.002013,    0.065823,  -13.364482,   -0.096078,   -0.005051,  360.354095,//扫描方式6
// -0.085470,   -0.000876,  334.023163,   -0.003390,    0.064725,   -6.211169,//扫描方式7
//};

volatile uint8_t ucXPT2046_TouchFlag = 0;



///**
//  * @brief  XPT2046 初始化函数
//  * @param  无
//  * @retval 无
//  */	
//void XPT2046_Init ( void )
//{
//  GPIO_InitTypeDef  GPIO_InitStructure;
//	 /* 开启GPIO时钟 */
//	__HAL_RCC_GPIOE_CLK_ENABLE();
//	__HAL_RCC_GPIOD_CLK_ENABLE();
// 
//  /* 模拟SPI GPIO初始化 */          
//  GPIO_InitStructure.Pin=XPT2046_SPI_CLK_PIN;
//  GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH ;	  
//  GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
//  HAL_GPIO_Init(XPT2046_SPI_CLK_PORT, &GPIO_InitStructure);

//  GPIO_InitStructure.Pin = XPT2046_SPI_MOSI_PIN;
//  HAL_GPIO_Init(XPT2046_SPI_MOSI_PORT, &GPIO_InitStructure);
//	

//	GPIO_InitStructure.Pin = XPT2046_SPI_CS_PIN; 
//  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH ;
//  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;      
//  HAL_GPIO_Init(XPT2046_SPI_CS_PORT, &GPIO_InitStructure); 
//	

//  GPIO_InitStructure.Pin = XPT2046_SPI_MISO_PIN; 
//  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH ;
//  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  //上拉输入
//	GPIO_InitStructure.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(XPT2046_SPI_MISO_PORT, &GPIO_InitStructure);

//  /* 拉低片选，选择XPT2046 */
//  XPT2046_CS_DISABLE();		
//								
//	//触摸屏触摸信号指示引脚，不使用中断
//  GPIO_InitStructure.Pin = XPT2046_PENIRQ_GPIO_PIN;       
//  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  //上拉输入
//	GPIO_InitStructure.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(XPT2046_PENIRQ_GPIO_PORT, &GPIO_InitStructure);
//}



/**
  * @brief  用于 XPT2046 的简单微秒级延时函数
  * @param  nCount ：延时计数值，单位为微妙
  * @retval 无
  */	
static void XPT2046_DelayUS ( __IO uint32_t ulCount )
{
	uint32_t i;
	for ( i = 0; i < ulCount; i ++ )
	{
		uint8_t uc = 12;     //设置值为12，大约延1微秒      
		while ( uc -- );     //延1微秒	
	}
}

/**
  * @brief  XPT2046 的写入命令
  * @param  ucCmd ：命令
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 无
  */
static void XPT2046_WriteCMD ( uint8_t ucCmd ) 
{
	uint8_t i;
	XPT2046_MOSI_0();	
	XPT2046_CLK_LOW();
	for ( i = 0; i < 8; i ++ ) 
	{
		if( ( ucCmd >> ( 7 - i ) ) & 0x01 )
		{
			XPT2046_MOSI_1()
		}else
		{
			XPT2046_MOSI_0();
		}
	  XPT2046_DelayUS ( 5 );
		XPT2046_CLK_HIGH();
	  XPT2046_DelayUS ( 5 );
		XPT2046_CLK_LOW();
	}
	
}
/**
  * @brief  XPT2046 的读取命令
  * @param  无
  * @retval 读取到的数据
  */
static uint16_t XPT2046_ReadCMD ( void ) 
{
	uint8_t i;
	uint16_t usBuf=0, usTemp;
	XPT2046_MOSI_0();
	XPT2046_CLK_HIGH();
	for ( i=0;i<12;i++ ) 
	{
		XPT2046_CLK_LOW();    
		usTemp = XPT2046_MISO();
		usBuf |= usTemp << ( 11 - i );
		XPT2046_CLK_HIGH();
	}
	return usBuf;
}


/**
  * @brief  对 XPT2046 选择一个模拟通道后，启动ADC，并返回ADC采样结果
  * @param  ucChannel
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 该通道的ADC采样结果
  */
static uint16_t XPT2046_ReadAdc ( uint8_t ucChannel )
{
	XPT2046_WriteCMD ( ucChannel );

  return 	XPT2046_ReadCMD ();
	
}
/**
  * @brief  读取 XPT2046 的X通道和Y通道的AD值（12 bit，最大是4096）
  * @param  sX_Ad ：存放X通道AD值的地址
  * @param  sY_Ad ：存放Y通道AD值的地址
  * @retval 无
  */
static void XPT2046_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad )  
{ 
	int16_t sX_Ad_Temp, sY_Ad_Temp; 
	
	sX_Ad_Temp = XPT2046_ReadAdc ( XPT2046_CHANNEL_X );

	XPT2046_DelayUS ( 1 ); 

	sY_Ad_Temp = XPT2046_ReadAdc ( XPT2046_CHANNEL_Y ); 
	
	
	* sX_Ad = sX_Ad_Temp; 
	* sY_Ad = sY_Ad_Temp; 
}

 




static uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate )
{
	uint8_t ucCount = 0, i;
	
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 10 ] = { { 0 },{ 0 } };  //坐标X和Y进行多次采样
	
	//存储采样中的最小值、最大值
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;


	/* 循环采样10次 */ 
	do					       				
	{		  
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );  
		
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;
		
		ucCount ++;  
		
	}	while ( ( XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel ) && ( ucCount < 10 ) );//用户点击触摸屏时即TP_INT_IN信号为低 并且 ucCount<10
	
	
	/*如果触笔弹起*/
	if ( XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel )
		ucXPT2046_TouchFlag = 0;			//中断标志复位

	
	/*如果成功采样10个样本*/
	if ( ucCount ==10 )		 					
	{
		lX_Max = lX_Min = sBufferArray [ 0 ] [ 0 ];
		lY_Max = lY_Min = sBufferArray [ 1 ] [ 0 ];       
		
		for ( i = 1; i < 10; i ++ )
		{
			if ( sBufferArray[ 0 ] [ i ] < lX_Min )
				lX_Min = sBufferArray [ 0 ] [ i ];
			
			else if ( sBufferArray [ 0 ] [ i ] > lX_Max )
				lX_Max = sBufferArray [ 0 ] [ i ];

		}
		
		for ( i = 1; i < 10; i ++ )
		{
			if ( sBufferArray [ 1 ] [ i ] < lY_Min )
				lY_Min = sBufferArray [ 1 ] [ i ];
			
			else if ( sBufferArray [ 1 ] [ i ] > lY_Max )
				lY_Max = sBufferArray [ 1 ] [ i ];

		}
		
		
		/*去除最小值和最大值之后求平均值*/
		pScreenCoordinate ->x =  ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] + sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + 
		                           sBufferArray [ 0 ] [ 5 ] + sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] + sBufferArray [ 0 ] [ 9 ] - lX_Min-lX_Max ) >> 3;
		
		pScreenCoordinate ->y =  ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] + sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + 
		                           sBufferArray [ 1 ] [ 5 ] + sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] + sBufferArray [ 1 ] [ 9 ] - lY_Min-lY_Max ) >> 3; 
		
		
		return 1;		
		
	}   	
	return 0;    	
}








   
/**
  * @brief  获取 XPT2046 触摸点（校准后）的坐标
  * @param  pDisplayCoordinate ：该指针存放获取到的触摸点坐标
  * @param  pTouchPara：坐标校准系数
  * @retval 获取情况
	*   该返回值为以下值之一：
  *     @arg 1 :获取成功
  *     @arg 0 :获取失败
  */
uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara )
{
	uint8_t ucRet = 1;           //若正常，则返回0
	
	strType_XPT2046_Coordinate strScreenCoordinate; 
	

  if ( XPT2046_ReadAdc_Smooth_XY ( & strScreenCoordinate ) )
  {    
		pDisplayCoordinate ->x = ( ( pTouchPara[LCD_SCAN_MODE].dX_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dX_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dX );        
		pDisplayCoordinate ->y = ( ( pTouchPara[LCD_SCAN_MODE].dY_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dY_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dY );

  }
	 
	else ucRet = 0;            //如果获取的触点信息有误，则返回0
		
  return ucRet;
} 





/**
  * @brief  触摸屏检测状态机
  * @retval 触摸状态
	*   该返回值为以下值之一：
  *     @arg TOUCH_PRESSED :触摸按下
  *     @arg TOUCH_NOT_PRESSED :无触摸
  */
uint8_t XPT2046_TouchDetect(void)
{ 
	static enumTouchState touch_state = XPT2046_STATE_RELEASE;
	static uint32_t i;
	uint8_t detectResult = TOUCH_NOT_PRESSED;
	
	switch(touch_state)
	{
		case XPT2046_STATE_RELEASE:	
			if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) //第一次出现触摸信号
			{
				touch_state = XPT2046_STATE_WAITING;
				detectResult =TOUCH_NOT_PRESSED;
				}
			else	//无触摸
			{
				touch_state = XPT2046_STATE_RELEASE;
				detectResult =TOUCH_NOT_PRESSED;
			}
			break;
				
		case XPT2046_STATE_WAITING:
				if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel)
				{
					 i++;
					//等待时间大于阈值则认为触摸被按下
					//消抖时间 = DURIATION_TIME * 本函数被调用的时间间隔
					//如在定时器中调用，每10ms调用一次，则消抖时间为：DURIATION_TIME*10ms
					if(i > DURIATION_TIME)		
					{
						i=0;
						touch_state = XPT2046_STATE_PRESSED;
						detectResult = TOUCH_PRESSED;
					}
					else												//等待时间累加
					{
						touch_state = XPT2046_STATE_WAITING;
						detectResult =	 TOUCH_NOT_PRESSED;					
					}
				}
				else	//等待时间值未达到阈值就为无效电平，当成抖动处理					
				{
				    i = 0;
            touch_state = XPT2046_STATE_RELEASE; 
						detectResult = TOUCH_NOT_PRESSED;
				}
		
			break;
		
		case XPT2046_STATE_PRESSED:	
				if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel)		//触摸持续按下
				{
					touch_state = XPT2046_STATE_PRESSED;
					detectResult = TOUCH_PRESSED;
				}
				else	//触摸释放
				{
					touch_state = XPT2046_STATE_RELEASE;
					detectResult = TOUCH_NOT_PRESSED;
				}
			break;			
		
		default:
				touch_state = XPT2046_STATE_RELEASE;
				detectResult = TOUCH_NOT_PRESSED;
				break;
	
	}		
	
	return detectResult;
}






/**
	* @brief   检测到触摸中断时调用的处理函数,通过它调用tp_down 和tp_up汇报触摸点
	*	@note 	 本函数需要在while循环里被调用，也可使用定时器定时调用
	*			例如，可以每隔5ms调用一次，消抖阈值宏DURIATION_TIME可设置为2，这样每秒最多可以检测100个点。
	*						可在XPT2046_TouchDown及XPT2046_TouchUp函数中编写自己的触摸应用
	* @param   none
	* @retval  none
	*/
void XPT2046_TouchEvenHandler(void )
{
	  static strType_XPT2046_Coordinate cinfo={-1,-1,-1,-1};
	
		if(XPT2046_TouchDetect() == TOUCH_PRESSED)
		{


			//获取触摸坐标
//			XPT2046_Get_TouchedPoint(&cinfo,strXPT2046_TouchPara);
			//输出调试信息到串口
			printf("x=%d,y=%d\r\n",cinfo.x,cinfo.y);
			/*更新触摸信息到pre xy*/
			cinfo.pre_x = cinfo.x; cinfo.pre_y = cinfo.y;  

		}
		else
		{
			/*触笔释放，把 xy 重置为负*/
			cinfo.x = -1;
			cinfo.y = -1; 
			cinfo.pre_x = -1;
			cinfo.pre_y = -1;
		}

}


/***************************end of file*****************************/


