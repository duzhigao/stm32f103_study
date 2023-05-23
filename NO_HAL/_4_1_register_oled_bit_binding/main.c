#define RCC_APB2ENR (*(volatile unsigned int *)(0x40021000+0x18))
#define GPIOB_CRL (*(volatile unsigned int *)(0x40010C00+0x00))
#define GPIOB_ODR (*(volatile unsigned int *)(0x40010C00+0x0C))

#define A(address,n) (((address&0xF0000000)+0x2000000)+((address&0xFFFFF)<<5)+(n<<2))  //算出对应位段别名区春存储单元的地址
#define BA(address,n) (*((volatile unsigned int *)(A(address,n))))	 //访问对应别名区的存储单元
#define PB_OUT(n) BA(0x40010C0C,n)
#define LED PB_OUT(1)

/*使用位段别名区来点灯*/
int main (void)
{
	/*使能PB模块的时钟*/
	RCC_APB2ENR |= (1 << 3);
	/*配置PB1为推挽输出，并配置速率*/
	GPIOB_CRL &= ~(0x11 << 4 * 1);//先清位，再写入
	GPIOB_CRL |= (0x1 << 5);
	/*配置PB1输出低电平*/
	//GPIOB_ODR &= ~(1 << 1);//开灯
	LED = 0; //开灯
	//volatile unsigned int d = (((0x40010C0C&0xF0000000)+0x2000000)+((0x40010C0C&0xFFFFF)<<5)+(1<<2));
	while(1);
	return 0;
}