#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
	
#define RCC_APB2ENR (*(volatile unsigned int *)(0x40021000+0x18))
#define GPIOB ((GPIO_TypeDef *)0x40010C00)

#define RCC_CR (*((uint32_t *)(0x40021000+0x00)))
#define RCC_CFGR (*((uint32_t *)(0x40021000+0x04)))
#define RCC_CIR (*((uint32_t *)(0x40021000+0x08)))
#define FLASH_ACR (*((uint32_t *)(0x40022000)))
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

void LED_Init(void);
void Delay(void);
int main(void)
{
	Stm32_Clock_Init();
	LED_Init();
	while(1)
	{
		GPIOB->ODR &= ~(1 << 1);
		Delay();
		GPIOB->ODR |= 1 << 1;
		Delay();
	}
}

/*延时函数定义*/ 
void Delay(void)
{
	uint16_t i,j;
	for(i=0;i<200;i++)
		for(j=0;j<10000;j++);
}
void LED_Init(void)
{	
	/*使能PB模块的时钟*/
	RCC_APB2ENR |= (1 << 3);
	/*配置PB1为推挽输出，并配置速率*/
	(GPIOB->CR)[0] &= ~(0x11 << 4 * 1);//先清位，再写入
	(GPIOB->CR)[0] |= (0x1 << 5);
	/*配置PB1输出低电平*/
	GPIOB->ODR |= 1 << 1;
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
		RCC_CFGR |= 7<<18; //PLL 9倍频
		
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