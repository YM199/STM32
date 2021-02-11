#include "iic.h"

void IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);

	GPIO_InitStructure.GPIO_Pin = IIC_SCL_Pin|IIC_SDA_Pin; 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(IIC_GPIO, &GPIO_InitStructure);	  
 	GPIO_SetBits(IIC_GPIO,IIC_SCL_Pin|IIC_SDA_Pin);	
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
	IIC_SDA_1();//先拉高SDA再拉高SCL，防止出现错误信号
	IIC_SCL_1();
	i2c_Delay();//延时保证SCL高电平时，SDA下降沿
	IIC_SDA_0();
	IIC_SCL_0();//如果不拉低SCL线，那么数据传输过程中的SDA的电平变换就会被认为是起始和结束信号
}

/*
 * @brief IIC停止信号
 */
void IIC_Stop(void)
{
	IIC_SDA_0();//先拉低SDA再拉高SCL，防止出现错误信号
	IIC_SCL_1();
	i2c_Delay();//延时保证SCL高电平时，SDA上升沿沿
	IIC_SDA_1();
	/*这里就不用拉低SCL了，因为IIC通讯已经结束*/
}
/*
 * @brief IIC主机等待应答
 */
void IIC_Wait_Ack(void)
{
	//一定要先拉高SDA，再拉高SCL。否者就可能变成起始信号和结束信号了
	IIC_SDA_1();
	IIC_SCL_1();
	while(IIC_Read_SDA());
	IIC_SCL_0();//IIC通讯没结束，拉低SCL线
}


/*
 * @brief IIC写字节
 * @para  待写入的字节数据
 */
void Write_IIC_Byte(u8 IIC_Byte)
{
	u8 i,data;
	data = IIC_Byte;
	for(i=0;i<8;i++)		
	{
		IIC_SCL_0();
		if(data&0x80)
			IIC_SDA_1();
		else 
			IIC_SDA_0();
		data=data<<1;
		IIC_SCL_1();
	}
	IIC_SCL_0();//通讯未结束，拉低SCL
}


