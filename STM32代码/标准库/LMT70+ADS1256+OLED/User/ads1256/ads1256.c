//-----------------------------------------------------------------
// 程序描述:
//      ADS1256单通道8路，差分4路测试
// 作    者: 凌智电子
// 开始日期: 2017-12-15
// 完成日期: 2017-01-04
// 修改日期: 
// 当前版本: V1.0.0
// 调试工具: 凌智STM32+FPGA电子系统设计开发板、2.8寸液晶屏、LZE_ST_LINK2、ADS1256_ADC_V1.0
// 说    明:

//-----------------------------------------------------------------
#include "stm32f10x.h"
#include "ads1256.h"
#include "./systick/bsp_SysTick.h"
#include "Delay.h"
/*
*********************************************************************************************************
*	函 数 名: ADS1256_Send8Bit
*	功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/
void ADS1256_Send8Bit(uint8_t data)
{
	uint8_t i;

	/* 连续发送多个字节时，需要延迟一下 */
	Delay_ns(250);
	Delay_ns(250);
	/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	for(i = 0; i < 8; i++)
	{
		if (data & 0x80)
		{
			DIN_H;
		}
		else
		{
			DIN_L;
		}
		SCLK_H;
		Delay_ns(250);
		data <<= 1;
		SCLK_L;			/* <----  ADS1256 是在SCK下降沿采样DIN数据, 数据必须维持 50nS */
		Delay_ns(250);
	}
}

/*********************************************************************************************************
*	函 数 名: ADS1256_Recive8Bit
*	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
uint8_t ADS1256_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;

	Delay_ns(250);
	Delay_ns(250);
	/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	for (i = 0; i < 8; i++)
	{
		SCLK_H;
		Delay_ns(250);
		read = read<<1;
		SCLK_L;
		if (DO_IS_H)
		{
			read++;
		}
		Delay_ns(250);
	}
	return read;
}

/*********************************************************************************************************
*	函 数 名: ADS1256_WriteReg
*	功能说明: 写指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_WriteReg(uint8_t RegID, uint8_t RegValue)
{
	CS_L;	/* SPI片选 = 0 */
	ADS1256_Send8Bit(CMD_WREG | RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	ADS1256_Send8Bit(0x00);		/* 寄存器个数 - 1, 此处写1个寄存器 */
	ADS1256_Send8Bit(RegValue);	/* 发送寄存器值 */
	CS_H;	/* SPI片选 = 1 */
}

/*********************************************************************************************************
*	函 数 名: ADS1256_ReadReg
*	功能说明: 读指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值。
*	返 回 值: 读到的寄存器值。
*********************************************************************************************************/
uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_L;	/* SPI片选 = 0 */
	ADS1256_Send8Bit(CMD_RREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	ADS1256_Send8Bit(0x00);	/* 寄存器个数 - 1, 此处读1个寄存器 */

	Delay_1us(1);	/* 必须延迟才能读取芯片返回数据 */

	read = ADS1256_Recive8Bit();	/* 读寄存器值 */
	CS_H;	/* SPI片选 = 1 */

	return read;
}

/*********************************************************************************************************
*	函 数 名: ADS1256_WriteCmd
*	功能说明: 发送单字节命令
*	形    参:  _cmd : 命令
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_WriteCmd(uint8_t cmd)
{
	CS_L;	/* SPI片选 = 0 */
	ADS1256_Send8Bit(cmd);
	CS_H;	/* SPI片选 = 1 */
}

/*********************************************************************************************************
*	函 数 名: ADS1256_CfgADC
*	功能说明: 配置ADC参数，增益和数据输出速率
*	形    参: gain : 增益
*			  _		drate : 数据输出速率
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_CfgADC(uint8_t gain,uint8_t drate)   
{
	ADS1256_WriteCmd(CMD_RESET);                //写复位指令
	ADS1256_WriteReg(REG_STATUS,0XF4);          //写状态，数据传输默认高位在前，启动矫正，禁止使用缓冲
	ADS1256_WriteCmd(CMD_SELFCAL);              //自校准
	Delay_1us(20);
	{
		uint8_t buf[4];
		/* 状态寄存器定义
			Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)

			Bit 3 ORDER: Data Output Bit Order
				0 = Most Significant Bit First (default)
				1 = Least Significant Bit First
			Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
			byte first. The ORDER bit only controls the bit order of the output data within the byte.

			Bit 2 ACAL : Auto-Calibration
				0 = Auto-Calibration Disabled (default)
				1 = Auto-Calibration Enabled
			When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
			the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
			values.

			Bit 1 BUFEN: Analog Input Buffer Enable
				0 = Buffer Disabled (default)
				1 = Buffer Enabled

			Bit 0 DRDY :  Data Ready (Read Only)
				This bit duplicates the state of the DRDY pin.

			ACAL=1使能自校准功能。当 PGA，BUFEEN, DRATE改变时会启动自校准
		*/
		buf[0] = (0 << 3) | (1 << 2) | (0 << 1);
		
		/* 高四位0表示AINP接 AIN0,  低四位8表示 AINN 固定接 AINCOM */
		buf[1] = 0x08;                      //通道设置选择
		
		/*	ADCON: A/D Control Register (Address 02h)
			Bit 7 Reserved, always 0 (Read Only)
			Bits 6-5 CLK1, CLK0 : D0/CLKOUT Clock Out Rate Setting
				00 = Clock Out OFF
				01 = Clock Out Frequency = fCLKIN (default)
				10 = Clock Out Frequency = fCLKIN/2
				11 = Clock Out Frequency = fCLKIN/4
				When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.

			Bits 4-2 SDCS1, SCDS0: Sensor Detect Current Sources
				00 = Sensor Detect OFF (default)
				01 = Sensor Detect Current = 0.5 μ A
				10 = Sensor Detect Current = 2 μ A
				11 = Sensor Detect Current = 10μ A
				The Sensor Detect Current Sources can be activated to verify  the integrity of an external sensor supplying a signal to the
				ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.

			Bits 2-0 PGA2, PGA1, PGA0: Programmable Gain Amplifier Setting
				000 = 1 (default)
				001 = 2
				010 = 4
				011 = 8
				100 = 16
				101 = 32
				110 = 64
				111 = 64
		*/
		buf[2] = (0 << 5) | (0 << 3) | (gain << 0);
		
		buf[3] = drate;	// DRATE_10SPS;	/* 选择数据输出速率 */
		CS_L;
		ADS1256_Send8Bit(CMD_WREG|0);          //写寄存器
		ADS1256_Send8Bit(0x03);                //连续写入4个数据
		ADS1256_Send8Bit(buf[0]);              //设置状态寄存器
		ADS1256_Send8Bit(buf[1]);              //设置输入通道参数
		ADS1256_Send8Bit(buf[2]);              //设置ADCON控制寄存器，增益
		ADS1256_Send8Bit(buf[3]);	             //设置数据速率
		CS_H;
	}
	Delay_1us(50);
}

/*********************************************************************************************************
*	函 数 名: ADS1256_ReadData
*	功能说明: 读取ADS1256端口的数据（即电压值）
*	形    参: 无
*	返 回 值: 读取的数据
*********************************************************************************************************/
uint32_t ADS1256_ReadData(void)
{
	
	uint32_t read = 0;

	CS_L;	/* SPI片选 = 0 */

	ADS1256_Send8Bit(CMD_RDATA);	/* 读数据的命令 */

	Delay_1us(1);	/* 必须延迟才能读取芯片返回数据 */
	
	
	/* 读采样结果，3个字节，高字节在前 */
	read = (uint32_t)ADS1256_Recive8Bit() << 16;
	read += ((uint32_t)ADS1256_Recive8Bit() << 8);
	read += ADS1256_Recive8Bit();

	CS_H;	/* SPI片选 = 1 */

	return read;
}

/*********************************************************************************************************
*	函 数 名: ADS1256_GetAdc
*	功能说明: 获取ADS1256指定通道的电压值
*	形    参: chaanal: 所选择的通道
*                    高四位表示同向通道选择，低四位表示反向通道选择，单通道输入时，channal的低四位为 1xxx
*	返 回 值: 读取的数据
*********************************************************************************************************/
uint32_t ADS1256_GetAdc(uint8_t channel)
{
	uint32_t read;
	read = 0;
	
// 	while(DRDY);           //当DRDY变为低电平时，数据开始传输
	ADS1256_WriteReg(REG_MUX,channel);       // 写入读取的通道
	
	ADS1256_WriteCmd(CMD_SYNC);              //同步A/D转换命令
	
// 	Delay_1us(1);
	ADS1256_WriteCmd(CMD_WAKEUP);            //完成SYNC并退出待机模式
	
	while(!DRDY_IS_L);                             // 等待数据转换完成	
 	read = 	ADS1256_ReadData();
	return read;	
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
