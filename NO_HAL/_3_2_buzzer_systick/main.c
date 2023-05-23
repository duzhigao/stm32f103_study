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
	Stm32_Clock_Init();/*ϵͳʱ��Ϊ72MHZ*/
	Buzzer_Init();
	while(1)
	{
		GPIOA->ODR |= 1 << 8;//��
		Delay(2000);
		GPIOA->ODR &= ~(1 << 8);//����
		Delay(2000);
	}
}

/*��ʱ��������*/ 
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
/*���� ms ����ʱ�������ú����������ʱΪ 1.864ms*/ 
void Delay_Tick(uint16_t ms)
{
	// ѡ��SystickΪHLK/8==9MHZ
	uint16_t count_ms = 9000;//1ms 14000������
	uint32_t total_count = count_ms * ms;
	if(total_count > 0xFFFFFF) //����һ��tick clock�������� 2��24�η�-1
		return;
	SYS_STICK->LOAD = total_count;//װ�������ĳ�ֵ������ֵ�Ĵ�����
	SYS_STICK->CTRL &= ~(1<<2);//Systick ѡ��HCLK/8
	SYS_STICK->CTRL |= 1;//��ʼ��ʱ
	while((SYS_STICK->CTRL & (1<<16)) ==0);//�ȴ�һ�ּ�������
	SYS_STICK->CTRL &= ~1;//һ�ּ����������رռ�����
	
}
void Buzzer_Init(void)
{	
	/*ʹ��PAģ���ʱ��*/
	RCC_APB2ENR |= (1 << 2);
	/*����PA8Ϊ�������������������*/
	(GPIOA->CR)[1] &= (~0xF);//����λ����д��
	(GPIOA->CR)[1] |= (0x1<<1);
	/*����PA1����͵�ƽ*/
	GPIOA->ODR &= ~(1 << 8);//����
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
		RCC_CFGR |= 10<<18; //PLL 8��Ƶ
		
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