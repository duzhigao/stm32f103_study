#define RCC_APB2ENR (*(volatile unsigned int *)(0x40021000+0x18))
#define GPIOB_CRL (*(volatile unsigned int *)(0x40010C00+0x00))
#define GPIOB_ODR (*(volatile unsigned int *)(0x40010C00+0x0C))

int main (void)
{
	/*使能PB模块的时钟*/
	RCC_APB2ENR |= (1 << 3);
	/*配置PB1为推挽输出，并配置速率*/
	GPIOB_CRL &= ~(0x11 << 4 * 1);//先清位，再写入
	GPIOB_CRL |= (0x1 << 5);
	/*配置PB1输出低电平*/
	GPIOB_ODR &= ~(1 << 1);
	while(1);
	return 0;
}