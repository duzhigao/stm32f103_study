#include "tim.h"
#include "stm32f103zet6.h"

void TIM3_PWM_Init(void)
{
	RCC_APB1ENR |= 1<<1;//TIM3模块使能
	TIM3->PSC = 7199;//分频7200，挂载在APB1下，72MHZ，CNT_CLK为10KHZ,1个脉冲0.1ms
	TIM3->ARR = 1999;//周期为2000个脉冲，200ms
	TIM3->CR1 |= 1<<4;//向下计数
	TIM3->CR1 |= 1<<7;//ARR的影子寄存器不和ARR直连，只有更新事件，影子寄存器的值才会到ARR
	
	TIM3->CCMR2 &=~3;//设置捕获比较通道为输出 
	TIM3->CCMR2 |= 1<<2;//当比较结果导致电平变化时，会有短暂延迟输出，设为1，将延迟缩短
	TIM3->CCMR2 |= 1<<3;//CCMR的影子寄存器不和CCMR直连，只有更新事件，影子寄存器的值才会到CCMR
	
	/*设置PWM模式为PWM模式1，CNT<CCR3时，OCREF为1*/
	TIM3->CCMR2 &=~(7<<4);
	TIM3->CCMR2 |= 6<<4;
	
	TIM3->CCR3 = 1000;//CNT为0~999为高电平 1000~1999为低电平
	TIM3->CCER &= ~(1<<9);//OC1REF 与OC1相同电平
	
	RCC_APB2ENR |= 1<<3;//使能GPIOB模块时钟
	/*设置为输出复用模式，速度为2MHZ*/
	GPIOB->CR[0] &=~0xF;
	GPIOB->CR[0]|=0xA;
}

void TIM_PWM_Start()
{
	TIM3->CCER |=1<<8;//使能通道
	TIM3->CR1 |=1;//开始计数
}
