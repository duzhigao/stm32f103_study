#include "sys.h"
#include "led.h"
#include "tim.h"
int main(void)
{
	System_Clock_Init();
	LED_Init();
	TIM3_Init();
	while(1)
	{
		LED_RED_TOGGLE();
		Delay_ms(1000);
	}
	
}