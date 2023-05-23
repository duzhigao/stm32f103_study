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

/*��ʱ��������*/ 
void Delay(void)
{
	uint16_t i,j;
	for(i=0;i<200;i++)
		for(j=0;j<10000;j++);
}
void LED_Init(void)
{	
	/*ʹ��PBģ���ʱ��*/
	RCC_APB2ENR |= (1 << 3);
	/*����PB1Ϊ�������������������*/
	(GPIOB->CR)[0] &= ~(0x11 << 4 * 1);//����λ����д��
	(GPIOB->CR)[0] |= (0x1 << 5);
	/*����PB1����͵�ƽ*/
	GPIOB->ODR |= 1 << 1;
}

/*ϵͳʱ�ӳ�ʼ����������*/ 
void Stm32_Clock_Init()
{
	/*ϵͳ������Ĭ��ʹ��HSIʱ�����Բ���*/
	//RCC_CR |= 1<0;//���� HSISON,�����ڲ����� RC ��
	//while((RCC_CR & 2) == 0); //��HSI�ȶ���
	RCC_CFGR = 0;//CFGR��λΪĬ��ֵ
	RCC_CR &=0xFCF4FFFF;//HSEON,CSSON,PLLON ����
	RCC_CR &=~(1<<18); //HSEBYP ����,�ⲿ������·
	RCC_CIR = 0;//��ֹ RCC ʱ���ж�
	Sys_Clock_Set();//����ʱ��
}

/*ʹ�ܸ����ⲿʱ�� HSE ��Ϊϵͳʱ��Դ������ PLL ���໷*/ 
uint8_t Sys_Clock_Set()
{
	uint16_t retry = 0;
	uint16_t status = 0;	
	RCC_CR |= 1<<16;
	while((RCC_CR & (1<<17)) == 0 && (retry<0x1FFF)) //�ȴ� HSE RDY
		retry++;
	if(retry == 0x1FFF)
		status =1;//HSE �޷�����
	else
	{
		RCC_CFGR |= 1<<10 ;//AHB����Ƶ ,APB1 2��Ƶ��APB2����Ƶ
		RCC_CFGR |= 1<<16;//PLL ʱ����ԴΪHSI  
		RCC_CFGR |= 7<<18; //PLL 9��Ƶ
		
		RCC_CR |= 1<<24; //����PLLCLK
		while((RCC_CR & (1<<25)) == 0);//��PLLCLK�ȶ���
		
		FLASH_ACR &= ~7;//��λ
		FLASH_ACR |= 2; //ѡ�������ȴ�ʱ��
		
		RCC_CFGR &= ~3; //��λ
		RCC_CFGR |= 2;//ѡ��HSE��Ϊ��PLL������ʱ��Դ����ʱHSI��û�йرգ�ֻ�ǲ�ʹ��Ϊ��PLL��ʱ��Դ����
		
		while( (RCC_CFGR & (3<<2)) !=(2<<2) );//�ȴ��� PLL ��Ϊϵͳʱ�ӳɹ�

	}
	return status;


}