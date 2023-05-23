#include "sys.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
/*�����1s����1sһֱѭ�����������KEY1�������жϺ�����exti.c����������*/
int main(void)
{
	System_Clock_Init();
	LED_Init();
	KEY_Init();
	EXTIX_Init();
	MY_NVIC_Config();
	while(1)
	{
		LED_RED = ON;
		Delay(1000);
		LED_RED = OFF;
		Delay(1000);
	}
}

