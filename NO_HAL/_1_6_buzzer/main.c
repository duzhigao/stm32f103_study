#define RCC_APB2RENR (*((volatile unsigned int *)(0x40021000 +0X18)))
#define GPIOA_CRH (*((volatile unsigned int *)(0x40010800+0x04)))
#define GPIOA_ODR (*((volatile unsigned int *)(0x40010800+0x0C)))
	
#define uint16_t unsigned short
#define uint8_t unsigned char
void Deylay(void);
void BEEP_Init(void);
int main(void)
{
	/*初始化*/
	BEEP_Init();
	/*GPIO_P8引脚为高电平，蜂鸣器响，低电平不响*/
	while(1)
	{
		GPIOA_ODR |= 1 << 8; //高电平
		Deylay();
		GPIOA_ODR &= ~(1 << 8); //低电平
	}

	return 0;
}
void BEEP_Init(void)
{
	/*使能GPIO_PORT_A的时钟*/
	RCC_APB2RENR |= 1 << 2 ;
	/*配置GPIO_P8引脚为输出引脚，为推挽输出,配置速率为2MHZ*/
	GPIOA_CRH &= ~0xF;
	GPIOA_CRH |= 1 << 1;
	/*关闭蜂鸣器*/
	GPIOA_ODR &= ~(1 << 8); //低电平
}

void Deylay(void)
{
	uint8_t i;
	uint16_t j;
	for( i = 0; i <200; i++)
		for(j = 0; j < 50000; j++)
			;
}
