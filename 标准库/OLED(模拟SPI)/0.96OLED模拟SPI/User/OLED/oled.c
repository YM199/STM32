#include "oled.h"
#include "font.h"
static void OLED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(OLED_RES_CLK|OLED_DC_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = OLED_RES_Pin|OLED_DC_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OLED_RES_PORT,&GPIO_InitStructure);	
	Software_SPI_Init();
	/*��λ�Ĵ���*/
	OLED_RES_0();
	SysTick_Delay_Ms(200);
	OLED_RES_1();
}

static void OLED_WriteCmd(u8 Cmd)
{
	SPI_CS_0();
	OLED_DC_Cmd();//DC���ŵ͵�ƽ��ʾ����
	Software_SPI_Write(Cmd);
	SPI_CS_1();
}

static void OLED_WriteData(u8 Cmd)
{
	SPI_CS_0();
	OLED_DC_Data();//DC���Ÿߵ�ƽ��ʾ����
	Software_SPI_Write(Cmd);
	SPI_CS_1();
}

void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WriteCmd(0XB0+i);//����ҳ��ַ 0XB0~0XB7
		OLED_WriteCmd(0X00);//������ʾλ�á��е͵�ַ
		OLED_WriteCmd(0X10);//������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)
		{
			OLED_WriteData(0);
		}
	}
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WriteCmd(0xb0+y);
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);
	OLED_WriteCmd((x&0x0f)|0x00); 
}   

void OLED_Init(void)
{
	OLED_GPIO_Init();
	OLED_WriteCmd(0XAE);//�ر�OLED���
	OLED_WriteCmd(0X00);//����ҳ��ַģʽ������ʼ��ַ��λ
	OLED_WriteCmd(0X10);//����ҳ��ַģʽ������ʼ��ַ��λ
	OLED_WriteCmd(0X40);//������Ļ��ʼ��
	
	OLED_WriteCmd(0X81);//���öԱȶ�(�൱����Ļ����)
	OLED_WriteCmd(0X00);//�Աȶ�����ֵ����Χ0X00-0XFF
	
	/*������������ʾ�����*/
	OLED_WriteCmd(0XA1);//������ɨ�跽��Ϊ������
	OLED_WriteCmd(0XC8);//������ɨ�跽��Ϊ���ݵ�λ��ǰ
	
	
	OLED_WriteCmd(0XA8);//���ø�����
	
	OLED_WriteCmd(0X3F);
	
	/*��ʾƫ��*/
	OLED_WriteCmd(0XD3);//������ʾƫ��
	OLED_WriteCmd(0x00);//��ƫ��
	
	/*����ʱ��*/
	OLED_WriteCmd(0XD5);//������ʾʱ�ӷ�Ƶֵ/��Ƶ��
	OLED_WriteCmd(0x80);//���÷�Ƶ�ȣ���ʱ������Ϊ100֡/��
	
	/*���*/
	OLED_WriteCmd(0XD9);//����Ԥ�������
	OLED_WriteCmd(0xF1);//1�����ʱ�Ӻ�15���ŵ�ʱ��
	
	OLED_WriteCmd(0XDA);//����������Ӳ������
	OLED_WriteCmd(0x12);//�����ɨ�跽���COM63��COM0(C8h), ��������/��ӳ��(DAh A[5]=1)

	OLED_WriteCmd(0XDB);//����VCOMH��ѹֵ
	OLED_WriteCmd(0X40);//����VCOMȡ��ѡ�񼶱�
	
	OLED_WriteCmd(0X20);//�����ڴ�Ѱַģʽ
	OLED_WriteCmd(0x02);//ҳ��ַѰַģʽ
	
	OLED_WriteCmd(0x8D);//��ɱ�����/����
	OLED_WriteCmd(0x14);//�ر�
	
	OLED_WriteCmd(0xA4);//������Ļ
	OLED_WriteCmd(0xA6);//����������ʾ������ת��1��ʾ��������
	OLED_WriteCmd(0xAF);//��OLED���
	
	OLED_Clear();//����
	OLED_Set_Pos(0,0);//������ʼ����
}

/*
 * @brief ��ʾһ������
 * @para  x ������ 
 * @para  y ������
*/
void OLED_ShowChar(u8 x,u8 y,u8 Char)
{
	u8 t;
	OLED_Set_Pos(x,y);
	for(t=0;t<16;t++)
	{
		OLED_WriteData(chinese[2*Char][t]);
  }
	OLED_Set_Pos(x,y+1);
  for(t=0;t<16;t++)
	{	
			OLED_WriteData(chinese[2*Char+1][t]);
  }			
}

/*
 * @brief ��ʾ�������
 * @para  x ������   
 * @para  y ������
 * @para  length ����ʾ���ֵĳ���
*/
void OLED_ShowCHinese(u8 x,u8 y,u8 length)
{
	u8 i;
	for(i=0;i<length;i++)
	{
		OLED_ShowChar(x,y,i);
		x += 16;
	}
}

/*
 * @brief ��ʾ8x16�ַ���
 * @para  x  ������
 * @para  y  ������
 * @para  ch ָ��
*/
void OLED_8x16Str(u8 x,u8 y,char *ch)
{
	u8 c=0,i=0,j=0;
	while(ch[j]!='\0')
	{
		c = ch[j]-32;
		if(x>120)//������ʾ
		{
			x=0;
			y++;
		}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		{
			OLED_WriteData(F8X16[c*16+i]);//ǰ8λ��ģ����
		}
		OLED_Set_Pos(x,y+1);//��һҳ
		for(i=0;i<8;i++)
		{
			OLED_WriteData(F8X16[c*16+i+8]);//ǰ8λ��ģ����
		}
		x+=8;//��һ���ַ�������
		j++;//�ַ������к�
	}
}

