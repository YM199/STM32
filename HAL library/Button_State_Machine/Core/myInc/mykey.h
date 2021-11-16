#ifndef __MYKEY_H__
#define __MYKEY_H__

#include "main.h"
#include "tim.h"
/* ����״̬����ʼ������ */
typedef struct{
	uint32_t GPIO_Pull;		/*������������ģʽ*/
	GPIO_TypeDef* GPIOx;	/*������Ӧ�Ķ˿�*/
	uint16_t GPIO_Pin_x;	/*����������*/
	uint8_t key_index;    /*����������*/
}Key_Init;

/* ����״̬��������״̬ */
typedef enum {
	KEY_NULL = 0u,/* �޶���*/
	KEY_SURE,      /* ȷ��״̬*/
	KEY_RAISE,     /* ����̧��*/
	KEY_PRESS,     /* ��������*/
	KEY_LONG       /* ����*/
}KEY_STATUS_LIST;

/*�������α�־*/
typedef FunctionalState KEY_ENABLE_STATUS;

/*����IO��ȡ��־*/
#define NPress_Down  GPIO_PIN_RESET/*����δ����*/
#define Press_Down   GPIO_PIN_SET  /*��������*/
typedef GPIO_PinState IO_STATUS_LIST;

/*��ȡIO��ƽ�ĺ��� ����ָ��  ��������?*/
static IO_STATUS_LIST KEY_ReadPin(Key_Init Key)
{
	return (IO_STATUS_LIST)HAL_GPIO_ReadPin(Key.GPIOx, Key.GPIO_Pin_x);
}

/*״̬����*/
typedef struct __KEY_COMPONENTS
{
	KEY_ENABLE_STATUS KEY_SHIELD;      /*�������Σ�DISABLE(0);���Σ�ENABLE(1);������*/
	uint8_t KEY_TIMECOUNT;             /*������������*/
	IO_STATUS_LIST KEY_FLAG;           /*�Ƿ��µı�־ 1��ʾ����*/
	IO_STATUS_LIST KEY_DOWN_LEVEL;     /*����ʱ������IOʵ�ʵĵ�ƽ*/
	KEY_STATUS_LIST KEY_CUR_STATUS;    /*����CUR״̬*/
	KEY_STATUS_LIST KEY_NEXT_STATUS;   /*����NEXT״̬*/
	KEY_STATUS_LIST KEY_EVENT;         /*�����¼�*/
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
	KEY0,
	KEY1,/*�û���ӵİ�ť����*/
	KEY_NUM,/*����Ҫ�еļ�¼��ť���������������*/
}KEY_LIST;

void KEY_Init ( void );
void Key_CallBack ( void );
#endif  /*__MYKEY_H__*/

