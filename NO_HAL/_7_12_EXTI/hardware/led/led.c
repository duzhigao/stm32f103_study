#include "led.h" 
#include "sys.h"
void LED_Init(void)
{
	RCC_APB2ENR |= 1<<3;//ʹ��GPIOBģ��
	GPIO_SET(GPIOB,1|(1<<5),2,0,0);//����PIN0 PIN5����
	LED_RED = OFF;
	LED_BLUE = OFF;
}