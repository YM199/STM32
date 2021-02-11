#include "iic.h"   

 /**
  * @brief  初始化IO
  * @param  无
  * @retval 无
  */
void IIC_GPIO_Config(void)
{		
    GPIO_InitTypeDef  GPIO_InitStruct;
    IIC_GPIO_CLK_ENABLE();													   
    GPIO_InitStruct.Pin = IIC_SCL_PIN | IIC_SDA_PIN;	
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;  
    /*设置引脚为上拉模式*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    /*设置引脚速率为高速 */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStruct);	
		IIC_SDA_SCL(GPIO_PIN_SET);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/*
 * @brief IIC起始信号
 */
void IIC_Start(void)
{
	IIC_SDA(GPIO_PIN_SET);//先拉高SDA再拉高SCL，防止出现错误信号
	IIC_SCL(GPIO_PIN_SET);
	i2c_Delay();//延时保证SCL高电平时，SDA下降沿
	IIC_SDA(GPIO_PIN_RESET);
	IIC_SCL(GPIO_PIN_RESET);//如果不拉低SCL线，那么数据传输过程中的SDA的电平变换就会被认为是起始和结束信号
}

/*
 * @brief IIC停止信号
 */
void IIC_Stop(void)
{
	IIC_SDA(GPIO_PIN_RESET);//先拉低SDA再拉高SCL，防止出现错误信号
	IIC_SCL(GPIO_PIN_SET);
	i2c_Delay();//延时保证SCL高电平时，SDA上升沿沿
	IIC_SDA(GPIO_PIN_SET);
	/*这里就不用拉低SCL了，因为IIC通讯已经结束*/
}

/*
 * @brief IIC主机等待应答
 */
void IIC_Wait_Ack(void)
{
	//一定要先拉高SDA，再拉高SCL。否者就可能变成起始信号和结束信号了
	IIC_SDA(GPIO_PIN_SET);
	IIC_SCL(GPIO_PIN_SET);
	while(IIC_Read_SDA());
	IIC_SCL(GPIO_PIN_RESET);//IIC通讯没结束，拉低SCL线
}


/*
 * @brief IIC写字节
 * @para  待写入的字节数据
 */
void IIC_Write_Byte(uint8_t IIC_Byte)
{
	uint8_t i,data;
	data = IIC_Byte;
	for(i=0;i<8;i++)		
	{
		IIC_SCL(GPIO_PIN_RESET);
		if(data&0x80)
			IIC_SDA(GPIO_PIN_SET);
		else 
			IIC_SDA(GPIO_PIN_RESET);
		data=data<<1;
		IIC_SCL(GPIO_PIN_SET);
	}
	IIC_SCL(GPIO_PIN_RESET);//通讯未结束，拉低SCL
}

void check_device(uint8_t addr)
{
	IIC_Start();
	IIC_Write_Byte(addr | write_bit);
	IIC_Wait_Ack();
	printf("检测到设备\n");
}
/*********************************************END OF FILE**********************/
