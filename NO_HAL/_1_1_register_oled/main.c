#define RCC_APB2ENR (*(volatile unsigned int *)(0x40021000+0x18))
#define GPIOB_CRL (*(volatile unsigned int *)(0x40010C00+0x00))
#define GPIOB_ODR (*(volatile unsigned int *)(0x40010C00+0x0C))

int main (void)
{
	/*ʹ��PBģ���ʱ��*/
	RCC_APB2ENR |= (1 << 3);
	/*����PB1Ϊ�������������������*/
	GPIOB_CRL &= ~(0x11 << 4 * 1);//����λ����д��
	GPIOB_CRL |= (0x1 << 5);
	/*����PB1����͵�ƽ*/
	GPIOB_ODR &= ~(1 << 1);
	while(1);
	return 0;
}