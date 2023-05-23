/*PB1*/
#define RCC_APB2ENR (*(volatile unsigned int *)(0x40021000+0x18))
#define GPIOB ((GPIO_TypeDef *)0x40010C00)

#define uint16_t unsigned short
#define uint32_t unsigned int
	
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
	LED_Init();
	while(1)
	{
		GPIOB->ODR &= ~(1 << 1);
		Delay();
		GPIOB->ODR |= 1 << 1;
		Delay();
	}
}

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