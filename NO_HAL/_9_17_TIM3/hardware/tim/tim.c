#include "tim.h"
#include "stm32f103zet6.h"

void TIM3_Init(void)
{
	RCC_APB1ENR |= 1<<1;//TIM3模块使能
	TIM3->PSC = 71;//分频72，挂载在APB1下，72MHZ，CNT_CLK为1MHZ,1个脉冲us
	TIM3->ARR = 1;//设为1，因为会单独设置CNT，防止在Delay_us中死循环
	TIM3->CR1 |= 1<<4;//向下计数
	TIM3->CR1 |= 1<<7;//ARR的影子寄存器不和ARR直连，只有更新事件，影子寄存器的值才会到ARR
}

void TIM3_Start(void)
{
	//使能计数器，开始计数
	TIM3->CR1 |= 1;
	
}
void TIM3_Stop(void)
{
	//失能计数器，停止计数
	TIM3->CR1 &= ~1;
}
void TIM3_Clear(void)
{
	TIM3->CNT = 0;//CNT清0
}

void Delay_ms(uint16_t ms)
{
	uint16_t i;
	for(i = 0;i<ms;i++)
		Delay_us(1000);
		
}
		
void Delay_us(uint16_t us)
{
	TIM3->CNT = us;//1个脉冲1us
	TIM3_Start();
	while(TIM3->CNT > 1); //因为ARR为1
	TIM3_Stop();
}