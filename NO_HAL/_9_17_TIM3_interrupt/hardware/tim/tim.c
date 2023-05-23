#include "tim.h"
#include "stm32f103zet6.h"
#include "sys.h"
#include "led.h"

void TIM3_Init_IT(void)
{
	RCC_APB1ENR |= 1<<1;//TIM3ģ��ʹ��
	TIM3->PSC = 7199;//��Ƶ72��������APB1�£�72MHZ��CNT_CLKΪ10KHZ,1������0.1ms
	TIM3->ARR = 9999;//����Ϊ1s
	TIM3->CR1 |= 1<<4;//���¼���
	TIM3->CR1 |= 1<<7;//ARR��Ӱ�ӼĴ�������ARRֱ����ֻ�и����¼���Ӱ�ӼĴ�����ֵ�ŻᵽARR
	
	NVIC_Config(29,3,3,0);//ʹ��TIM3�жϺţ����������ȼ���Ϊ3��������ռ���ȼ�Ϊ3�������ȼ�Ϊ0
	
}

void TIM3_Start_IT(void)
{
	TIM3->DIER |= 1;//set DIER��UIEλ��ʹ�ܸ����ж�
	TIM3->CR1 |= 1;//set CR1��CENλ��ʹ�ܼ���������ʼ����
	
}
void TIM3_Stop_IT(void)
{
	TIM3->CR1 &= ~1;//reset CR1��CENλ��ʧ�ܼ�������ֹͣ����
}

/*ע�ⶨʱ����Uart��ͬ������ر��ж�*/
void TIM3_IRQHandler(void)
{
	if((TIM3->SR & 1) == 1)//�鿴UIFλ��ȷ�ϸ����¼�������
	{
		TIM3->SR &= ~1;//���UIFλ
		LED_RED_TOGGLE();
	}
}

