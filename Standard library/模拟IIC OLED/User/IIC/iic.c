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
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}
/*
 * @brief IIC��ʼ�ź�
 */
void IIC_Start(void)
{
	IIC_SDA_1();//������SDA������SCL����ֹ���ִ����ź�
	IIC_SCL_1();
	i2c_Delay();//��ʱ��֤SCL�ߵ�ƽʱ��SDA�½���
	IIC_SDA_0();
	IIC_SCL_0();//���������SCL�ߣ���ô���ݴ�������е�SDA�ĵ�ƽ�任�ͻᱻ��Ϊ����ʼ�ͽ����ź�
}

/*
 * @brief IICֹͣ�ź�
 */
void IIC_Stop(void)
{
	IIC_SDA_0();//������SDA������SCL����ֹ���ִ����ź�
	IIC_SCL_1();
	i2c_Delay();//��ʱ��֤SCL�ߵ�ƽʱ��SDA��������
	IIC_SDA_1();
	/*����Ͳ�������SCL�ˣ���ΪIICͨѶ�Ѿ�����*/
}
/*
 * @brief IIC�����ȴ�Ӧ��
 */
void IIC_Wait_Ack(void)
{
	//һ��Ҫ������SDA��������SCL�����߾Ϳ��ܱ����ʼ�źźͽ����ź���
	IIC_SDA_1();
	IIC_SCL_1();
	while(IIC_Read_SDA());
	IIC_SCL_0();//IICͨѶû����������SCL��
}


/*
 * @brief IICд�ֽ�
 * @para  ��д����ֽ�����
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
	IIC_SCL_0();//ͨѶδ����������SCL
}


