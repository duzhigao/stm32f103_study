#include "exti.h"
#include "key.h"
#include "sys.h"
#include "cortex_m3.h"
#include "delay.h"
#include "led.h"
extern a;
void EXTIX_Init(void)
{
	KEY_Init();//使能KEY1的PA模块的时钟并将PA0设置为悬空输入
	EXTI_Config('A',0,1);//参数1只能是“A~G”,必须大写，参数2为外部中断线下标，参数3(0=上升沿触发，1=下降沿触发)
}

void MY_NVIC_Config(void)
{
	NVIC_Config(6,4,3,0);//使能中断号为6（EXTI0）的中断，并设置优先级组，设置中断号6的优先级
}

/*中断函数中不要用滴答定时器，滴答定时器的中断优先级优先*/
void EXTI0_IRQHandler(void)
{
	//EXTI->PR |= 1;//清除PR标志位，如果不清除，就会重复执行中断函数
	Delay(20);//消抖
	if(KEY1 == PRESSED)
	{
		LED_BLUE = ON;
		Delay(2000);
		LED_BLUE = OFF;
		Delay(2000);
	}	
}