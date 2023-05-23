#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
	
#define RCC_APB2ENR (*(volatile unsigned int *)(0x40021000+0x18))
#define GPIOA ((GPIO_TypeDef *)0x40010800)

#define RCC_CR (*((uint32_t *)(0x40021000+0x00)))
#define RCC_CFGR (*((uint32_t *)(0x40021000+0x04)))
#define RCC_CIR (*((uint32_t *)(0x40021000+0x08)))
#define FLASH_ACR (*((uint32_t *)(0x40022000)))

#define SYS_STICK ((SysTick_Type *)0xE000E010)
void Stm32_Clock_Init();
uint8_t Sys_Clock_Set();

typedef struct
{
    volatile uint32_t CR[2]; //CRL and CRH
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
}GPIO_TypeDef;

typedef struct
{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
}SysTick_Type;


void Buzzer_Init(void);
void Delay(uint16_t);
void Delay_Tick (uint16_t);
int main(void)
{
	Stm32_Clock_Init();/*系统时钟为72MHZ*/
	Buzzer_Init();
	while(1)
	{
		GPIOA->ODR |= 1 << 8;//响
		Delay(2000);
		GPIOA->ODR &= ~(1 << 8);//不响
		Delay(2000);
	}
}

/*延时函数定义*/ 
void Delay(uint16_t ms)
{
	uint16_t i = ms / 500;
	uint16_t j = ms % 500;
	while(i>0)
	{
		Delay_Tick(500);
		i--;
	}
	Delay_Tick(j);
}
/*定义 ms 级延时函数，该函数的最大延时为 1.864ms*/ 
void Delay_Tick(uint16_t ms)
{
	// 选择Systick为HLK/8==9MHZ
	uint16_t count_ms = 9000;//1ms 14000个脉冲
	uint32_t total_count = count_ms * ms;
	if(total_count > 0xFFFFFF) //大于一个tick clock的最大计数 2的24次方-1
		return;
	SYS_STICK->LOAD = total_count;//装计数器的初值到重载值寄存器中
	SYS_STICK->CTRL &= ~(1<<2);//Systick 选择HCLK/8
	SYS_STICK->CTRL |= 1;//开始计时
	while((SYS_STICK->CTRL & (1<<16)) ==0);//等待一轮计数结束
	SYS_STICK->CTRL &= ~1;//一轮计数结束，关闭计数器
	
}
void Buzzer_Init(void)
{	
	/*使能PA模块的时钟*/
	RCC_APB2ENR |= (1 << 2);
	/*配置PA8为推挽输出，并配置速率*/
	(GPIOA->CR)[1] &= (~0xF);//先清位，再写入
	(GPIOA->CR)[1] |= (0x1<<1);
	/*配置PA1输出低电平*/
	GPIOA->ODR &= ~(1 << 8);//不响
}

/*系统时钟初始化函数定义*/ 
void Stm32_Clock_Init()
{
	/*系统启动后默认使用HSI时钟所以不用*/
	//RCC_CR |= 1<0;//设置 HSISON,开启内部高速 RC 振荡
	//while((RCC_CR & 2) == 0); //待HSI稳定后
	RCC_CFGR = 0;//CFGR复位为默认值
	RCC_CR &=0xFCF4FFFF;//HSEON,CSSON,PLLON 清零
	RCC_CR &=~(1<<18); //HSEBYP 清零,外部晶振不旁路
	RCC_CIR = 0;//禁止 RCC 时钟中断
	Sys_Clock_Set();//设置时钟
}

/*使能高速外部时钟 HSE 作为系统时钟源，开启 PLL 锁相环*/ 
uint8_t Sys_Clock_Set()
{
	uint16_t retry = 0;
	uint16_t status = 0;	
	RCC_CR |= 1<<16;
	while((RCC_CR & (1<<17)) == 0 && (retry<0x1FFF)) //等待 HSE RDY
		retry++;
	if(retry == 0x1FFF)
		status =1;//HSE 无法就绪
	else
	{
		RCC_CFGR |= 1<<10 ;//AHB不分频 ,APB1 2分频，APB2不分频
		RCC_CFGR |= 1<<16;//PLL 时钟来源为HSI  
		RCC_CFGR |= 10<<18; //PLL 8倍频
		
		RCC_CR |= 1<<24; //启动PLLCLK
		while((RCC_CR & (1<<25)) == 0);//待PLLCLK稳定后
		
		FLASH_ACR &= ~7;//清位
		FLASH_ACR |= 2; //选择两个等待时钟
		
		RCC_CFGR &= ~3; //清位
		RCC_CFGR |= 2;//选择HSE作为主PLL的输入时钟源，此时HSI并没有关闭，只是不使用为主PLL的时钟源而已
		
		while( (RCC_CFGR & (3<<2)) !=(2<<2) );//等待主 PLL 作为系统时钟成功

	}
	return status;


}