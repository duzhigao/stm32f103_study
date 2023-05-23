#include "tim.h"
#include "stm32f103zet6.h"
#include "sys.h"
#include "led.h"

void TIM3_Init_IT(void)
{
	RCC_APB1ENR |= 1<<1;//TIM3模块使能
	TIM3->PSC = 7199;//分频72，挂载在APB1下，72MHZ，CNT_CLK为10KHZ,1个脉冲0.1ms
	TIM3->ARR = 9999;//周期为1s
	TIM3->CR1 |= 1<<4;//向下计数
	TIM3->CR1 |= 1<<7;//ARR的影子寄存器不和ARR直连，只有更新事件，影子寄存器的值才会到ARR
	
	NVIC_Config(29,3,3,0);//使能TIM3中断号，并设置优先级组为3，设置抢占优先级为3，子优先级为0
	
}

void TIM3_Start_IT(void)
{
	TIM3->DIER |= 1;//set DIER的UIE位，使能更新中断
	TIM3->CR1 |= 1;//set CR1的CEN位，使能计数器，开始计数
	
}
void TIM3_Stop_IT(void)
{
	TIM3->CR1 &= ~1;//reset CR1的CEN位，失能计数器，停止计数
}

/*注意定时器与Uart不同，不会关闭中断*/
void TIM3_IRQHandler(void)
{
	if((TIM3->SR & 1) == 1)//查看UIF位，确认更新事件发生了
	{
		TIM3->SR &= ~1;//清除UIF位
		LED_RED_TOGGLE();
	}
}

