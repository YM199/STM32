#include "iic.h"   

 /**
  * @brief  ��ʼ��IO
  * @param  ��
  * @retval ��
  */
void IIC_GPIO_Config(void)
{		
    GPIO_InitTypeDef  GPIO_InitStruct;
    IIC_GPIO_CLK_ENABLE();													   
    GPIO_InitStruct.Pin = IIC_SCL_PIN | IIC_SDA_PIN;	
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;  
    /*��������Ϊ����ģʽ*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    /*������������Ϊ���� */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStruct);	
		IIC_SDA_SCL(GPIO_PIN_SET);
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
	IIC_SDA(GPIO_PIN_SET);//������SDA������SCL����ֹ���ִ����ź�
	IIC_SCL(GPIO_PIN_SET);
	i2c_Delay();//��ʱ��֤SCL�ߵ�ƽʱ��SDA�½���
	IIC_SDA(GPIO_PIN_RESET);
	IIC_SCL(GPIO_PIN_RESET);//���������SCL�ߣ���ô���ݴ�������е�SDA�ĵ�ƽ�任�ͻᱻ��Ϊ����ʼ�ͽ����ź�
}

/*
 * @brief IICֹͣ�ź�
 */
void IIC_Stop(void)
{
	IIC_SDA(GPIO_PIN_RESET);//������SDA������SCL����ֹ���ִ����ź�
	IIC_SCL(GPIO_PIN_SET);
	i2c_Delay();//��ʱ��֤SCL�ߵ�ƽʱ��SDA��������
	IIC_SDA(GPIO_PIN_SET);
	/*����Ͳ�������SCL�ˣ���ΪIICͨѶ�Ѿ�����*/
}

/*
 * @brief IIC�����ȴ�Ӧ��
 */
void IIC_Wait_Ack(void)
{
	//һ��Ҫ������SDA��������SCL�����߾Ϳ��ܱ����ʼ�źźͽ����ź���
	IIC_SDA(GPIO_PIN_SET);
	IIC_SCL(GPIO_PIN_SET);
	while(IIC_Read_SDA());
	IIC_SCL(GPIO_PIN_RESET);//IICͨѶû����������SCL��
}


/*
 * @brief IICд�ֽ�
 * @para  ��д����ֽ�����
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
	IIC_SCL(GPIO_PIN_RESET);//ͨѶδ����������SCL
}

void check_device(uint8_t addr)
{
	IIC_Start();
	IIC_Write_Byte(addr | write_bit);
	IIC_Wait_Ack();
	printf("��⵽�豸\n");
}
/*********************************************END OF FILE**********************/
