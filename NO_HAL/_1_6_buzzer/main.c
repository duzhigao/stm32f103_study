#define RCC_APB2RENR (*((volatile unsigned int *)(0x40021000 +0X18)))
#define GPIOA_CRH (*((volatile unsigned int *)(0x40010800+0x04)))
#define GPIOA_ODR (*((volatile unsigned int *)(0x40010800+0x0C)))
	
#define uint16_t unsigned short
#define uint8_t unsigned char
void Deylay(void);
void BEEP_Init(void);
int main(void)
{
	/*��ʼ��*/
	BEEP_Init();
	/*GPIO_P8����Ϊ�ߵ�ƽ���������죬�͵�ƽ����*/
	while(1)
	{
		GPIOA_ODR |= 1 << 8; //�ߵ�ƽ
		Deylay();
		GPIOA_ODR &= ~(1 << 8); //�͵�ƽ
	}

	return 0;
}
void BEEP_Init(void)
{
	/*ʹ��GPIO_PORT_A��ʱ��*/
	RCC_APB2RENR |= 1 << 2 ;
	/*����GPIO_P8����Ϊ������ţ�Ϊ�������,��������Ϊ2MHZ*/
	GPIOA_CRH &= ~0xF;
	GPIOA_CRH |= 1 << 1;
	/*�رշ�����*/
	GPIOA_ODR &= ~(1 << 8); //�͵�ƽ
}

void Deylay(void)
{
	uint8_t i;
	uint16_t j;
	for( i = 0; i <200; i++)
		for(j = 0; j < 50000; j++)
			;
}
