#include "key.h"
#include "delay.h"
#include "sys.h"
//key1-PA0
//key2-PG15
//key3-PC13
//key4-PE3

/* ����ʶ����.�а������·��ذ�����ֵ��û�а������·��� 0��
���صİ�����ֵ�밴���Ĺ�ϵΪ��KEY1=1;KEY2=2;KEY3=3;KEY_UP=4. */ 
uint8_t Scan_Key(void)
{
	static uint8_t key_flag = 1; //�������ڵ���״̬Ϊ1������״̬Ϊ0����ֹ�ظ�����
	//static���εľֲ�����
	//����г�ʼֵ����data�����ڳ�ʼ��ȫ�ֱ���ʱ��ʼ��
	//���û�г�ʼֵ�������.bss�Σ��ڳ�ʼ��ȫ�ֱ���ʱ��ʼ��Ϊ0

	/*��������ոմ��ڵ���״̬���������а���*/
	if((KEY1 == PRESSED || KEY2 == PRESSED || KEY3 == PRESSED || KEY4 == PRESSED) && key_flag == 1)
	{
		Delay(10);//��ʱ 10ms,��������
		if(KEY1 == PRESSED || KEY2 == PRESSED || KEY3 == PRESSED || KEY4 == PRESSED)
		{
			key_flag = 0;//��� 10ms ���������󻹰��£�˵������İ���
			if(KEY1 == PRESSED) return 1;
			if(KEY2 == PRESSED) return 2;
			if(KEY3 == PRESSED) return 3;
			if(KEY4 == PRESSED) return 4;
			
		}
	}
	
	/*�԰�����������жϣ��������ղ��ǰ��£������ǵ�����˵��������*/
	if((KEY1 == NO_PRESSED || KEY2 == NO_PRESSED || KEY3 == NO_PRESSED || KEY4 == NO_PRESSED) && key_flag == 0)
	{
		Delay(10);
		if(KEY1 == NO_PRESSED || KEY2 == NO_PRESSED || KEY3 == NO_PRESSED || KEY4 == NO_PRESSED)
			key_flag = 1;
	}
	return 0;
}

void KEY_Init(void)
{
	RCC_APB2ENR |=(1<<2)|(1<<4)|(1<<6)|(1<<8); //ʹ��GPIO_A C E G ģ��ʱ��
	GPIO_SET(GPIOA,1,0,1,0); //���ݵ�·ͼ�������룬��������������
	GPIO_SET(GPIOC,1<<13,0,1,0);
	GPIO_SET(GPIOE,1<<3,0,1,0);
	GPIO_SET(GPIOG,1<<15,0,1,0);
}