#include "led.h"
#include "sys.h"
#include "typedef.h"
#include "stm32f103zet6.h"
#include "led.h"
void LED_Init(void)
{
	RCC_APB2ENR |= 1<<3;  //使能GPIOB
	GPIO_SET(GPIOB,PIN(0)|PIN(5),2,0,0); //配置引脚
	LED_BLUE = 1;//不亮
	LED_RED = 1;
}