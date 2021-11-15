/*   ������־
V1.0   ����˹��ܺ����ı�д
V1.1   �������ʾ�������һλ����Ϊ0�����⡣��2018.12.24��
V1.2   ɾ�����ַ��ӵĴ��룬��ߴ���Ŀ���ֲ�ԡ�
V1.3   �Ż��˺���ȡģ��ʽ�����������޸ĵĸ���ճ��PCtoLCD2002���ɵ���ģ
V1.4   �޸���һЩBUG�������˴��������ַ�����ʹ�÷���˵��
*/

/* oled.h �ļ��а����������ã���򿪲鿴 */
#include "oled.h"

#include "oled_bmp.h"
#include "oled_font.h"

//�ַ���
u8 string[16];

//�����IIC�ӿ�
#if OLED_INTERFACE == 1
/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{
   GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);		
   GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
   GPIO_ResetBits(SDA_GPIO,SDA_GPIO_Pin);
   GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{  
   GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin); 
   GPIO_ResetBits(SDA_GPIO,SDA_GPIO_Pin);
	GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
   GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
}
/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(u8 IIC_Byte)
{
	u8 i;
	for(i=0;i<8;i++)		
	{
		GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
		if(IIC_Byte & 0x80)
			GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
		else
			GPIO_ResetBits(SDA_GPIO,SDA_GPIO_Pin);
		GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
		IIC_Byte<<=1;
	}
	GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
	GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
	GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
	GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
}
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	//дһ���ֽں���
	//dat Ҫд������ݻ�����
	//CMD ����/�����־λ 0���� 1����
	if(cmd==0)
	{
		IIC_Start();
		Write_IIC_Byte(0x78);         
		Write_IIC_Byte(0x00);		
		Write_IIC_Byte(dat); 
		IIC_Stop();
	}
	else
	{
		IIC_Start();
		Write_IIC_Byte(0x78);		
		Write_IIC_Byte(0x40);			
		Write_IIC_Byte(dat);
		IIC_Stop();		
	}
} 
//�����SPI�ӿ�
#else
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	//дһ���ֽں���
	//dat Ҫд������ݻ�����
	//CMD ����/�����־λ 0���� 1����
	u8 i;
	if(cmd)
		GPIO_SetBits(DC_GPIO,DC_GPIO_Pin);
	else 
		GPIO_ResetBits(DC_GPIO,DC_GPIO_Pin);
	for(i=0;i<8;i++)
	{
		GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
		if(dat&0x80)
			GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
		else
			GPIO_ResetBits(SDA_GPIO,SDA_GPIO_Pin);
		GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
		dat<<=1;
	}
	GPIO_SetBits(DC_GPIO,DC_GPIO_Pin);
} 
#endif

void OLED_Set_Pos(u8 x,u8 y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD);
} 

void OLED_Display_On(void)
{
	//����OLED��ʾ
	OLED_WR_Byte(0x8d,OLED_CMD);
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xaf,OLED_CMD);

}
void OLED_Display_Off(void)
{
	//�ر�OLED��ʾ
	OLED_WR_Byte(0x8d,OLED_CMD);
	OLED_WR_Byte(0x10,OLED_CMD);
	OLED_WR_Byte(0xaf,OLED_CMD);	

}
void OLED_Clear(void)
{
	//����
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte(0xb0+i,OLED_CMD);
		OLED_WR_Byte(0x02,OLED_CMD);
		OLED_WR_Byte(0x10,OLED_CMD);	
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
	}
}

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size)
{
	//��ָ���ط���ʾһ���ַ������������ַ�
	//x��0~127
	//y��0~63
	//size��ѡ������16*16��8*6
	u8 c=0,i=0;
	c=chr-' ';
	if(x>127)
	{
		x=0;
		y=y+2;
	}
	if(size == 16)
	{
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		{
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);			
		} 
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		{
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		
	} 
	else
	{
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		{
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		}
	}	
}

u32 oled_pow(u8 m,u8 n)
{
	//m^n����
	u32 result = 1;
	while(n--)result*=m;
	return result;
}

void OLED_ShowNum(u8 x,u8 y,long num,u8 len,u8 size)
{ 
	//����:��ʾ����
	//x,y :�������	
	//num:+-65535;	                        
	//len :���ֵ�λ����������һλ
	//size:�����С
	u32 t,temp1,temp2,temp3,flag; 
	if(num<0)
	{
		flag=1;      //������־λ��1
		num=-num;   //���ֱ�����
	}
	for(t=0;t<len;t++)
	{
		temp1=num/oled_pow(10,len-t-1);
		temp2=temp1%10;
		if(flag==1)
		{
			temp3 = num/oled_pow(10,len-t-2);
		}
		if(temp1!=0)
		{
			OLED_ShowChar(x,y,'0'+temp2,size);
			
		}
		else  
		{  
			if(t==len-1&&temp2==0)
			{							
			   OLED_ShowChar(x,y,'0',size);
			}
			else
			{
				if(flag==1&&temp3!=0)      //�����Ǹ���
				{
					OLED_ShowChar(x,y,'-',size);  //�������
					flag=0;                       //�������������־λ��0
				}
				else
				{
					OLED_ShowChar(x,y,' ',size);
				}	
			}
		}		
		x+=8;
	}

}

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size)
{
	//��ʾһ�����ִ����ַ��Ŵ�
	//����\r���Ի���
	//д���º�����Ҫ���еǼ�
	u8 x0=x;
	u16 k;
	while(*chr!='\0')
	{
		if((*chr)<128)
		{
			if(*chr==13)
			{
				x=x0;
				y+=size/8;
			}
			else
			{
				OLED_ShowChar(x,y,*chr,size);
				x+=8;
				if(x>120)
				{
					x=0;
					y+=size/8;
				}			
			} 
			chr++;
		}
		else if(size==16)        //�����Ƿ�Ϊ16
		{
			for(k=0;k<hz16_num;k++)  //��������
			{
				if ((hz16[k].Index[0]==*(chr))&&(hz16[k].Index[1]==*(chr+1)))   //�Ƿ����ѵǼǺ���
			   { 
					u8 t;
					OLED_Set_Pos(x,y);
					for(t=0;t<16;t++)
					{
						OLED_WR_Byte(hz16[k].Msk[t],OLED_DATA);
						
					}
					OLED_Set_Pos(x,y+1);
					for(t=0;t<16;t++)
					{
						OLED_WR_Byte(hz16[k].Msk[t+16],OLED_DATA);
					}
				
				}
				
			} 
			if(x>112)
			{
				x=0;
				y+=2;
			}				
			chr+=2;x+=16;
			
		}
		else       //������󣬺��Ժ���
		{
		   chr+=2;
		}
	}
}
void OLED_Draw12864BMP(u8 BMP[])
{
	//��ʾ��ʾBMPͼƬ128��64
	u16 j=0;
	u8 x,y;
	for(y=0;y<8;y++)
	{
		OLED_Set_Pos(0,y);
		for(x=0;x<128;x++)
		{
			#if DISPLAY_MODE
			OLED_WR_Byte(~BMP[j++],OLED_DATA);
			#else
			OLED_WR_Byte(BMP[j++],OLED_DATA);
			#endif
		}
	}
}
void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
	//��ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
	u16 j=0;
	u8 x,y;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
		{
			OLED_WR_Byte(BMP[j++],OLED_DATA);
		}
	}
}

void OLED_Init(void)
{
	//��ʼ����Ҫ�õ���IO,��ͷ�ļ��޸ļ���
	//ִ�г�ʼ��OLED����
	GPIO_InitTypeDef GPIO_InitStructure;
	//SCL  D0
	RCC_APB2PeriphClockCmd(SCL_GPIOClock,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SCL_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCL_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
	//SDA  D1
	RCC_APB2PeriphClockCmd(SDA_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SDA_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
	
//�����SPI�ӿ�
#if   OLED_INTERFACE == 2	      
	//RST
	RCC_APB2PeriphClockCmd(RST_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = RST_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RST_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(RST_GPIO,RST_GPIO_Pin);
  //DC
	RCC_APB2PeriphClockCmd(DC_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = DC_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DC_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(DC_GPIO,DC_GPIO_Pin);	
	//CS
	RCC_APB2PeriphClockCmd(CS_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = CS_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CS_GPIO,&GPIO_InitStructure);	
	GPIO_ResetBits(CS_GPIO,CS_GPIO_Pin);	
#endif

//�����Ҫʹ��IO�ڹ���
#if OLED_IO_to_POWER == 1
  //VCC
	RCC_APB2PeriphClockCmd(VCC_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = VCC_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(VCC_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(VCC_GPIO,VCC_GPIO_Pin);	
	//GND
	RCC_APB2PeriphClockCmd(GND_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GND_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GND_GPIO,&GPIO_InitStructure);	
	GPIO_ResetBits(GND_GPIO,GND_GPIO_Pin);
#endif

//�����SPI�ӿ�
#if   OLED_INTERFACE == 2	
	GPIO_SetBits(RST_GPIO,RST_GPIO_Pin);
	GPIO_ResetBits(RST_GPIO,RST_GPIO_Pin);
	GPIO_SetBits(RST_GPIO,RST_GPIO_Pin);	
#endif

						  
	OLED_WR_Byte(0xAE,OLED_CMD);//�ر���ʾ
	OLED_WR_Byte(0x02,OLED_CMD);//[1:0],00���е�ַģʽ;01��
															//�е�ַģʽ;10,ҳ��ַģʽ;Ĭ�� 10;
	OLED_WR_Byte(0x10,OLED_CMD);//���ø��е�ַ
	OLED_WR_Byte(0x40,OLED_CMD);//������ʾ��ʼ�� [5:0],����
	OLED_WR_Byte(0x81,OLED_CMD);//�Աȶ�����
	OLED_WR_Byte(0xCF,OLED_CMD);//�趨SEG�����������
	OLED_WR_Byte(0xA1,OLED_CMD);//���ض�������,bit0:0,0->0;1,0->127;     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//���� COM ɨ�跽��;bit3:0,��ͨģʽ;1,
															//�ض���ģʽ COM[N-1]->COM0;N:����·��   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA8,OLED_CMD);//��������·��
	OLED_WR_Byte(0x3f,OLED_CMD);//Ĭ�� 0X3F(1/64)
	OLED_WR_Byte(0x81,OLED_CMD);//�Աȶ�����
	OLED_WR_Byte(0xfF,OLED_CMD);//1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD3,OLED_CMD);////������ʾƫ��(0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(0x80,OLED_CMD);//[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xD9,OLED_CMD);//����Ԥ�������
	OLED_WR_Byte(0xF1,OLED_CMD);//��Ԥ�����Ϊ15��ʱ�Ӻͷŵ���Ϊ1��ʱ��
	OLED_WR_Byte(0xDA,OLED_CMD);//���� COM Ӳ����������
	OLED_WR_Byte(0x12,OLED_CMD);//[5:4]����
	OLED_WR_Byte(0xDB,OLED_CMD);//���� VCOMH ��ѹ����
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//�����ڴ��ַģʽ (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//[1:0],00���е�ַģʽ;01��
															//�е�ַģʽ;10,ҳ��ַģʽ;Ĭ�� 10;
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�����/����
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	#if DISPLAY_MODE == 1
	OLED_WR_Byte(0xA7,OLED_CMD);//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ 
	#else
	OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	
	#endif
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 	
	OLED_Clear();
	OLED_Set_Pos(0,0);	
}






