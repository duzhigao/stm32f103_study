#include "sys.h"
#include "stm32f103zet6.h"
#include "cortex_m3.h""
/*设置systemclk为PLLCLK 72MHZ，APB1为36MHZ APB2为72MHZ*/
void System_Clock_Init(void)
{
	RCC_CR |= 1;//使能HSI clock
	while((RCC_CR & 2) == 0);//等待HSI时钟稳定
	RCC_CR &= 0xFCF4FFFF;//将RCC_CR HSE PLL CSS相关位置为0
	RCC_CR &=~(1<<18); //HSEBYP 清零,外部晶振不旁路
	RCC_CFGR = 0;//将RCC_CFRG复位
	RCC_CIR = 0;//禁止 RCC 时钟中断
	System_Clock_Set();//设置systemclk为PLLCLK 72MHZ，APB1为36MHZ APB2为72MHZ
}

void System_Clock_Set(void)
{
	RCC_CR |= 1<<16;//使能HSE clock
	while((RCC_CR & (1<<17)) == 0);//等待HSE clock稳定
	RCC_CFGR |= 1 <<16;//设置PLL CLOCK的来源为HSE
	
	RCC_CFGR &= ~(0XF << 18);//清位
	RCC_CFGR |= 7 << 18;//设置PLL CLOCK的倍数为9
	
	RCC_CR |= 1 <<24;//开启PLL CLOCK时钟
	while((RCC_CR & 1<<25)==0);//待PLL CLOCK稳定后
	
	RCC_CFGR &= ~(7<<8);//清位
	RCC_CFGR |= (1<<10);//配置AHB不分频,APB1 2分频,APB2 不分频
	
	
	FLASH_ACR &= ~7;//清位
	FLASH_ACR |= 2; //选择两个等待时钟
	
	RCC_CFGR &= ~3;//清位
	RCC_CFGR |= 2;//选择系统时钟为PLL CLOCK
	
	while((RCC_CFGR & (3<<2)) != (2<<2));//等待切换成功
}
/*
**函数功能：设置GPIO的模式 速度（输出） 电路驱动方式   上拉下拉电阻
**函数参数：pin			8个bit数,bit n表示第几个引脚，为1则对配置该引脚
			mode		0-输入 1-输出且10MHZ 2-输出2MHZ  3-输出50MHZ
			mode_cnf	输入模式
							0-模拟 1-浮空 2-上拉下拉
						输出模式
							0-推挽输出	1-开漏输出 2-复用推挽 3-复用开漏
			updwn		仅在输入模式
							0-下拉无上拉
							1-上拉无下拉
							
			
*/
void GPIO_SET(GPIO_TypeDef* GPIOX,uint16_t pin,uint8_t mode,uint8_t mode_cnf,uint8_t updwn)
{
	uint8_t n = 0; //n表示第几位
	for(;n<16;n++)
	{
		if((pin & (1<<n)) == (1<<n)) //第几位为1，则对该位进行GPIO引脚配置
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
			if(mode == 0)	//输出模式没有上下拉电阻 ODR没有用，用于区分输入模式是否用上下拉
			{
				GPIOX->ODR = updwn;
			}
		}
		
	}
	
}


static uint8_t delay_times_semaphore = 0; //该次delay是否为中断中的第二次进入，可以理解为Systick这个资源的信号量
/*定义 ms 级延时函数，该函数的最大延时为 1.864ms*/ 
void Delay_Tick(uint16_t ms)
{
	uint32_t last_counter = 0;
	if(delay_times_semaphore != 0){
		 last_counter = SYS_STICK->VAL;
	}
	// 选择Systick为HLK/8==9MHZ
	uint16_t count_ms = 9000;//1ms 14000个脉冲
	uint32_t total_count = count_ms * ms;
	if(total_count > 0xFFFFFF || total_count == 0) //大于一个tick clock的最大计数 2的24次方-1
		return;
	SYS_STICK->LOAD = total_count;//装计数器的初值到重载值寄存器中
	SYS_STICK->VAL = 0x00;//写VAL时，无论写什么，都会将VAL置为0，将CTRL的标志位清0
	SYS_STICK->CTRL &= ~(1<<2);//Systick 选择HCLK/8
	SYS_STICK->CTRL |= 1;//开始计时
	delay_times_semaphore++;
	while((SYS_STICK->CTRL & (1<<16)) ==0);//等待一轮计数结束
	delay_times_semaphore--;
	if(delay_times_semaphore == 0) 
	{
		SYS_STICK->CTRL &= ~1;//一轮计数结束，关闭计数器
	}
	/*中断走这个分支，不停止计数器，让计数器装载剩余的次数*/
	else
	{
		SYS_STICK->CTRL &= ~1;//关闭计数器
		SYS_STICK->LOAD = last_counter;//装载上一次的值
		SYS_STICK->VAL = 0x00;//为了让VAL装载LOAD，写VAL清0（无论给VAL写什么值，VAL都会被清0，且CTRL的标志位会被清零）
		SYS_STICK->CTRL |= 1;//开启计时
	}
		
	
}

/*NVIC_Config()函数的作用是设置外部中断EXTIx的中断信号输入线、配置EXTIx的中断
是采用上升沿触发还是下降沿触发(0=上升沿触发，1=下降沿触发)、中断允许*/
void EXTI_Config(uint8_t GPIO_ch,uint8_t exti_x,uint8_t trigger)
{
	/*EXTI线配置引脚*/
	uint8_t index_EXTICR = exti_x / 4;
	uint8_t offset_EXTICR = (exti_x % 4) * 4;
	RCC_APB2ENR |= 1;//使能AFIO时钟
	AFIO_EXTICR[index_EXTICR] &= ~(0XF << offset_EXTICR);//清位
	AFIO_EXTICR[index_EXTICR] |= (GPIO_ch - 'A') << offset_EXTICR;//EXTI线配置引脚
	
	/*设置EXTI线的边沿检测*/
	if(trigger == 1)
		EXTI->FTSR |= 1 << exti_x; //下降沿触发
	else
	{
		EXTI->RTSR |= 1 << exti_x; //上升沿触发
	}
	
	/*设置EXTI线的屏蔽寄存器*/
	EXTI->IMR |= 1 << exti_x;//不屏蔽
	

	/*EXTI_PENDING相应位会被硬件置为1表示有中断信号过来了*/

}
void NVIC_Config(uint8_t Channel,uint8_t Group,uint8_t PreemptionPriority,uint8_t SubPriority)
{
	/*设置NVIC的ISER 使能某个外部中断*/
	uint8_t index_ISER = Channel / 32;
	uint8_t offset_ISER = Channel % 32;
	ISER[index_ISER] |= 1 << offset_ISER;
	
	/*设置优先级分组*/
	NVIC_PriorityGroupConfig(Group);
	
	/*设置优先级*/
	IP[Channel] = 0; //清位
	IP[Channel] |= SubPriority;//子优先级
	IP[Channel] |= PreemptionPriority << (Group+1); //抢占式优先级

}

void NVIC_PriorityGroupConfig(uint8_t group)
{
	uint32_t tmp = *AIRCR;
	tmp &= 0x0000F8FF;//清空key和group
	tmp |= (0x05fa<<16)&(group << 8);
	*AIRCR = tmp;
}