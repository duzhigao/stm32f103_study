#include "exti.h"
#include "key.h"
#include "sys.h"
#include "cortex_m3.h"
#include "delay.h"
#include "led.h"
extern a;
void EXTIX_Init(void)
{
	KEY_Init();//ʹ��KEY1��PAģ���ʱ�Ӳ���PA0����Ϊ��������
	EXTI_Config('A',0,1);//����1ֻ���ǡ�A~G��,�����д������2Ϊ�ⲿ�ж����±꣬����3(0=�����ش�����1=�½��ش���)
}

void MY_NVIC_Config(void)
{
	NVIC_Config(6,4,3,0);//ʹ���жϺ�Ϊ6��EXTI0�����жϣ����������ȼ��飬�����жϺ�6�����ȼ�
}

/*�жϺ����в�Ҫ�õδ�ʱ�����δ�ʱ�����ж����ȼ�����*/
void EXTI0_IRQHandler(void)
{
	//EXTI->PR |= 1;//���PR��־λ�������������ͻ��ظ�ִ���жϺ���
	Delay(20);//����
	if(KEY1 == PRESSED)
	{
		LED_BLUE = ON;
		Delay(2000);
		LED_BLUE = OFF;
		Delay(2000);
	}	
}