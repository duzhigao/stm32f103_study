#include "typedef.h"
#include  "sys.h"
#include "led.h"
#include "delay.h"
#include "stm32f103zet6.h"
//PB0ºì PB5À¶
int main(void)
{
	System_Clock_Init();//³õÊ¼»¯ÏµÍ³Ê±ÖÓÎª144MHZ
	LED_Init();
	while(1)
	{
		LED_BLUE = 1; //À¶µÆÏ¨Ãð
		LED_RED = 0; //ºìµÆÁÁ
		Delay(10000);//ÑÓÊ±10s
		LED_RED = 1; //ºìµÆÏ¨Ãð
		LED_BLUE = 0; //À¶µÆÁÁ
		Delay(10000);
	}

}