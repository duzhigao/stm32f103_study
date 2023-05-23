#include "tim.h"
#include "stm32f103zet6.h"

void TIM3_PWM_Init(void)
{
	RCC_APB1ENR |= 1<<1;//TIM3ģ��ʹ��
	TIM3->PSC = 7199;//��Ƶ7200��������APB1�£�72MHZ��CNT_CLKΪ10KHZ,1������0.1ms
	TIM3->ARR = 1999;//����Ϊ2000�����壬200ms
	TIM3->CR1 |= 1<<4;//���¼���
	TIM3->CR1 |= 1<<7;//ARR��Ӱ�ӼĴ�������ARRֱ����ֻ�и����¼���Ӱ�ӼĴ�����ֵ�ŻᵽARR
	
	TIM3->CCMR2 &=~3;//���ò���Ƚ�ͨ��Ϊ��� 
	TIM3->CCMR2 |= 1<<2;//���ȽϽ�����µ�ƽ�仯ʱ�����ж����ӳ��������Ϊ1�����ӳ�����
	TIM3->CCMR2 |= 1<<3;//CCMR��Ӱ�ӼĴ�������CCMRֱ����ֻ�и����¼���Ӱ�ӼĴ�����ֵ�ŻᵽCCMR
	
	/*����PWMģʽΪPWMģʽ1��CNT<CCR3ʱ��OCREFΪ1*/
	TIM3->CCMR2 &=~(7<<4);
	TIM3->CCMR2 |= 6<<4;
	
	TIM3->CCR3 = 1000;//CNTΪ0~999Ϊ�ߵ�ƽ 1000~1999Ϊ�͵�ƽ
	TIM3->CCER &= ~(1<<9);//OC1REF ��OC1��ͬ��ƽ
	
	RCC_APB2ENR |= 1<<3;//ʹ��GPIOBģ��ʱ��
	/*����Ϊ�������ģʽ���ٶ�Ϊ2MHZ*/
	GPIOB->CR[0] &=~0xF;
	GPIOB->CR[0]|=0xA;
}

void TIM_PWM_Start()
{
	TIM3->CCER |=1<<8;//ʹ��ͨ��
	TIM3->CR1 |=1;//��ʼ����
}
