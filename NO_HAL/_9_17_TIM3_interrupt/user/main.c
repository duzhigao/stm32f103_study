#include "sys.h"
#include "led.h"
#include "tim.h"
int main(void)
{
	System_Clock_Init();
	LED_Init();
	TIM3_Init_IT();
	TIM3_Start_IT();
	while(1);
}