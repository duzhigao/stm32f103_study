#include "typedef.h"
#include  "sys.h"
#include "led.h"
#include "delay.h"
#include "stm32f103zet6.h"
//PB0�� PB5��
int main(void)
{
	System_Clock_Init();//��ʼ��ϵͳʱ��Ϊ144MHZ
	LED_Init();
	while(1)
	{
		LED_BLUE = 1; //����Ϩ��
		LED_RED = 0; //�����
		Delay(10000);//��ʱ10s
		LED_RED = 1; //���Ϩ��
		LED_BLUE = 0; //������
		Delay(10000);
	}

}