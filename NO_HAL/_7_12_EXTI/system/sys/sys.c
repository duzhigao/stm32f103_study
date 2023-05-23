#include "sys.h"
#include "stm32f103zet6.h"
#include "cortex_m3.h""
/*����systemclkΪPLLCLK 72MHZ��APB1Ϊ36MHZ APB2Ϊ72MHZ*/
void System_Clock_Init(void)
{
	RCC_CR |= 1;//ʹ��HSI clock
	while((RCC_CR & 2) == 0);//�ȴ�HSIʱ���ȶ�
	RCC_CR &= 0xFCF4FFFF;//��RCC_CR HSE PLL CSS���λ��Ϊ0
	RCC_CR &=~(1<<18); //HSEBYP ����,�ⲿ������·
	RCC_CFGR = 0;//��RCC_CFRG��λ
	RCC_CIR = 0;//��ֹ RCC ʱ���ж�
	System_Clock_Set();//����systemclkΪPLLCLK 72MHZ��APB1Ϊ36MHZ APB2Ϊ72MHZ
}

void System_Clock_Set(void)
{
	RCC_CR |= 1<<16;//ʹ��HSE clock
	while((RCC_CR & (1<<17)) == 0);//�ȴ�HSE clock�ȶ�
	RCC_CFGR |= 1 <<16;//����PLL CLOCK����ԴΪHSE
	
	RCC_CFGR &= ~(0XF << 18);//��λ
	RCC_CFGR |= 7 << 18;//����PLL CLOCK�ı���Ϊ9
	
	RCC_CR |= 1 <<24;//����PLL CLOCKʱ��
	while((RCC_CR & 1<<25)==0);//��PLL CLOCK�ȶ���
	
	RCC_CFGR &= ~(7<<8);//��λ
	RCC_CFGR |= (1<<10);//����AHB����Ƶ,APB1 2��Ƶ,APB2 ����Ƶ
	
	
	FLASH_ACR &= ~7;//��λ
	FLASH_ACR |= 2; //ѡ�������ȴ�ʱ��
	
	RCC_CFGR &= ~3;//��λ
	RCC_CFGR |= 2;//ѡ��ϵͳʱ��ΪPLL CLOCK
	
	while((RCC_CFGR & (3<<2)) != (2<<2));//�ȴ��л��ɹ�
}
/*
**�������ܣ�����GPIO��ģʽ �ٶȣ������ ��·������ʽ   ������������
**����������pin			8��bit��,bit n��ʾ�ڼ������ţ�Ϊ1������ø�����
			mode		0-���� 1-�����10MHZ 2-���2MHZ  3-���50MHZ
			mode_cnf	����ģʽ
							0-ģ�� 1-���� 2-��������
						���ģʽ
							0-�������	1-��©��� 2-�������� 3-���ÿ�©
			updwn		��������ģʽ
							0-����������
							1-����������
							
			
*/
void GPIO_SET(GPIO_TypeDef* GPIOX,uint16_t pin,uint8_t mode,uint8_t mode_cnf,uint8_t updwn)
{
	uint8_t n = 0; //n��ʾ�ڼ�λ
	for(;n<16;n++)
	{
		if((pin & (1<<n)) == (1<<n)) //�ڼ�λΪ1����Ը�λ����GPIO��������
		{
			uint8_t i =0;
			uint8_t ishigh=0;
			if(n <= 7)
			{//GPIOX_CRL
				i = n;
				ishigh = 0;
			}
			else
			{	
			//GPIOX_CRH
				i = n - 8;
				ishigh =1;
			}
			GPIOX->CR[ishigh] &= ~(15 << (i*4));
			GPIOX->CR[ishigh] |= mode << (i*4);
			GPIOX->CR[ishigh] |= mode_cnf << ((i*4)+2);
			if(mode == 0)	//���ģʽû������������ ODRû���ã�������������ģʽ�Ƿ���������
			{
				GPIOX->ODR = updwn;
			}
		}
		
	}
	
}


static uint8_t delay_times_semaphore = 0; //�ô�delay�Ƿ�Ϊ�ж��еĵڶ��ν��룬�������ΪSystick�����Դ���ź���
/*���� ms ����ʱ�������ú����������ʱΪ 1.864ms*/ 
void Delay_Tick(uint16_t ms)
{
	uint32_t last_counter = 0;
	if(delay_times_semaphore != 0){
		 last_counter = SYS_STICK->VAL;
	}
	// ѡ��SystickΪHLK/8==9MHZ
	uint16_t count_ms = 9000;//1ms 14000������
	uint32_t total_count = count_ms * ms;
	if(total_count > 0xFFFFFF || total_count == 0) //����һ��tick clock�������� 2��24�η�-1
		return;
	SYS_STICK->LOAD = total_count;//װ�������ĳ�ֵ������ֵ�Ĵ�����
	SYS_STICK->VAL = 0x00;//дVALʱ������дʲô�����ὫVAL��Ϊ0����CTRL�ı�־λ��0
	SYS_STICK->CTRL &= ~(1<<2);//Systick ѡ��HCLK/8
	SYS_STICK->CTRL |= 1;//��ʼ��ʱ
	delay_times_semaphore++;
	while((SYS_STICK->CTRL & (1<<16)) ==0);//�ȴ�һ�ּ�������
	delay_times_semaphore--;
	if(delay_times_semaphore == 0) 
	{
		SYS_STICK->CTRL &= ~1;//һ�ּ����������رռ�����
	}
	/*�ж��������֧����ֹͣ���������ü�����װ��ʣ��Ĵ���*/
	else
	{
		SYS_STICK->CTRL &= ~1;//�رռ�����
		SYS_STICK->LOAD = last_counter;//װ����һ�ε�ֵ
		SYS_STICK->VAL = 0x00;//Ϊ����VALװ��LOAD��дVAL��0�����۸�VALдʲôֵ��VAL���ᱻ��0����CTRL�ı�־λ�ᱻ���㣩
		SYS_STICK->CTRL |= 1;//������ʱ
	}
		
	
}

/*NVIC_Config()�����������������ⲿ�ж�EXTIx���ж��ź������ߡ�����EXTIx���ж�
�ǲ��������ش��������½��ش���(0=�����ش�����1=�½��ش���)���ж�����*/
void EXTI_Config(uint8_t GPIO_ch,uint8_t exti_x,uint8_t trigger)
{
	/*EXTI����������*/
	uint8_t index_EXTICR = exti_x / 4;
	uint8_t offset_EXTICR = (exti_x % 4) * 4;
	RCC_APB2ENR |= 1;//ʹ��AFIOʱ��
	AFIO_EXTICR[index_EXTICR] &= ~(0XF << offset_EXTICR);//��λ
	AFIO_EXTICR[index_EXTICR] |= (GPIO_ch - 'A') << offset_EXTICR;//EXTI����������
	
	/*����EXTI�ߵı��ؼ��*/
	if(trigger == 1)
		EXTI->FTSR |= 1 << exti_x; //�½��ش���
	else
	{
		EXTI->RTSR |= 1 << exti_x; //�����ش���
	}
	
	/*����EXTI�ߵ����μĴ���*/
	EXTI->IMR |= 1 << exti_x;//������
	

	/*EXTI_PENDING��Ӧλ�ᱻӲ����Ϊ1��ʾ���ж��źŹ�����*/

}
void NVIC_Config(uint8_t Channel,uint8_t Group,uint8_t PreemptionPriority,uint8_t SubPriority)
{
	/*����NVIC��ISER ʹ��ĳ���ⲿ�ж�*/
	uint8_t index_ISER = Channel / 32;
	uint8_t offset_ISER = Channel % 32;
	ISER[index_ISER] |= 1 << offset_ISER;
	
	/*�������ȼ�����*/
	NVIC_PriorityGroupConfig(Group);
	
	/*�������ȼ�*/
	IP[Channel] = 0; //��λ
	IP[Channel] |= SubPriority;//�����ȼ�
	IP[Channel] |= PreemptionPriority << (Group+1); //��ռʽ���ȼ�

}

void NVIC_PriorityGroupConfig(uint8_t group)
{
	uint32_t tmp = *AIRCR;
	tmp &= 0x0000F8FF;//���key��group
	tmp |= (0x05fa<<16)&(group << 8);
	*AIRCR = tmp;
}