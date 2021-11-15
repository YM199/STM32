#ifndef __MYKEY_H__
#define __MYKEY_H__

#include "main.h"

/* ����״̬����ʼ������ */
typedef struct{
	uint32_t GPIO_Pull;		/*������������ģʽ*/
	GPIO_TypeDef* GPIOx;	/*������Ӧ�Ķ˿�*/
	uint16_t GPIO_Pin_x;	/*����������*/
	uint8_t key_nox;
}Key_Init;

/* ����״̬��������״̬ */
typedef enum _KEY_STATUS_LIST{
	KEY_NULL  = 0x00, /* �޶���*/
	KEY_SURE  = 0x01, /* ȷ��״̬*/
	KEY_RAISE = 0x02, /* ����̧��*/
	KEY_PRESS = 0x04, /* ��������*/
	KEY_LONG  = 0x08, /* ����*/
}KEY_STATUS_LIST;

/*�������α�־*/
typedef FunctionalState KEY_ENABLE_STATUS;

/*����IO��ȡ��־*/
#define LOW_LEVEL  GPIO_PIN_RESET
#define HIGH_LEVEL GPIO_PIN_SET
typedef GPIO_PinState IO_STATUS_LIST;

/*��ȡIO��ƽ�ĺ��� ����ָ��  ��������?*/
static IO_STATUS_LIST KEY_ReadPin(Key_Init Key)
{
	return (IO_STATUS_LIST)HAL_GPIO_ReadPin(Key.GPIOx, Key.GPIO_Pin_x);
}

/*״̬����*/
typedef struct __KEY_COMPONENTS
{
	KEY_ENABLE_STATUS KEY_SHIELD; /*�������Σ�DISABLE(0);���Σ�ENABLE(1);������*/
	uint8_t KEY_TIMECOUNT;        /*������������*/
	IO_STATUS_LIST KEY_FLAG;      /*�Ƿ��µı�־ 1��ʾ����*/
	IO_STATUS_LIST KEY_DOWN_LEVEL;/*����ʱ������IOʵ�ʵĵ�ƽ*/
	KEY_STATUS_LIST KEY_STATUS;   /*����״̬*/
	KEY_STATUS_LIST KEY_EVENT;    /*�����¼�*/
	IO_STATUS_LIST (*READ_PIN) (Key_Init Key);/*��IO��ƽ����������һ������ָ��*/
}KEY_COMPONENTS;


/*������,������һ������*/
typedef struct
{
	Key_Init Key_Board;      /*�̳г�ʼ������*/
	KEY_COMPONENTS KeyStatus;/*�̳�״̬������*/
}Key_Config;

/*����ע���*/
typedef enum
{
	KEY1,
	KEY2,/*�û���ӵİ�ť����*/
	KEY_NUM,/*����Ҫ�еļ�¼��ť���������������*/
}KEY_LIST;
#endif  /*__MYKEY_H__*/

