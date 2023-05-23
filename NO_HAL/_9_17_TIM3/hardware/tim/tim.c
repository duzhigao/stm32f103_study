#include "tim.h"
#include "stm32f103zet6.h"

void TIM3_Init(void)
{
	RCC_APB1ENR |= 1<<1;//TIM3ģ��ʹ��
	TIM3->PSC = 71;//��Ƶ72��������APB1�£�72MHZ��CNT_CLKΪ1MHZ,1������us
	TIM3->ARR = 1;//��Ϊ1����Ϊ�ᵥ������CNT����ֹ��Delay_us����ѭ��
	TIM3->CR1 |= 1<<4;//���¼���
	TIM3->CR1 |= 1<<7;//ARR��Ӱ�ӼĴ�������ARRֱ����ֻ�и����¼���Ӱ�ӼĴ�����ֵ�ŻᵽARR
}

void TIM3_Start(void)
{
	//ʹ�ܼ���������ʼ����
	TIM3->CR1 |= 1;
	
}
void TIM3_Stop(void)
{
	//ʧ�ܼ�������ֹͣ����
	TIM3->CR1 &= ~1;
}
void TIM3_Clear(void)
{
	TIM3->CNT = 0;//CNT��0
}

void Delay_ms(uint16_t ms)
{
	uint16_t i;
	for(i = 0;i<ms;i++)
		Delay_us(1000);
		
}
		
void Delay_us(uint16_t us)
{
	TIM3->CNT = us;//1������1us
	TIM3_Start();
	while(TIM3->CNT > 1); //��ΪARRΪ1
	TIM3_Stop();
}