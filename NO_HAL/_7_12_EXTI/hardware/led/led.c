#include "led.h" 
#include "sys.h"
void LED_Init(void)
{
	RCC_APB2ENR |= 1<<3;//使能GPIOB模块
	GPIO_SET(GPIOB,1|(1<<5),2,0,0);//配置PIN0 PIN5引脚
	LED_RED = OFF;
	LED_BLUE = OFF;
}