#define RCC_APB2ENR (*(volatile unsigned int *)(0x40021000+0x18))
#define GPIOB_CRL (*(volatile unsigned int *)(0x40010C00+0x00))
#define GPIOB_ODR (*(volatile unsigned int *)(0x40010C00+0x0C))

#define A(address,n) (((address&0xF0000000)+0x2000000)+((address&0xFFFFF)<<5)+(n<<2))  //�����Ӧλ�α��������洢��Ԫ�ĵ�ַ
#define BA(address,n) (*((volatile unsigned int *)(A(address,n))))	 //���ʶ�Ӧ�������Ĵ洢��Ԫ
#define PB_OUT(n) BA(0x40010C0C,n)
#define LED PB_OUT(1)

/*ʹ��λ�α����������*/
int main (void)
{
	/*ʹ��PBģ���ʱ��*/
	RCC_APB2ENR |= (1 << 3);
	/*����PB1Ϊ�������������������*/
	GPIOB_CRL &= ~(0x11 << 4 * 1);//����λ����д��
	GPIOB_CRL |= (0x1 << 5);
	/*����PB1����͵�ƽ*/
	//GPIOB_ODR &= ~(1 << 1);//����
	LED = 0; //����
	//volatile unsigned int d = (((0x40010C0C&0xF0000000)+0x2000000)+((0x40010C0C&0xFFFFF)<<5)+(1<<2));
	while(1);
	return 0;
}