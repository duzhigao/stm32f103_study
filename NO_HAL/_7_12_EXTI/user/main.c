#include "sys.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
/*红灯亮1s，灭1s一直循环，如果按下KEY1，进入中断函数（exti.c），蓝灯亮*/
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

