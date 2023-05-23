#include "uart.h"
#include "sys.h"
#include "stm32f103zet6.h"
#include "stdio.h"
/*TX -- PA9

**RX -- PA10
*���ò�����Ϊ115200
*�ֳ�Ϊ8��ֹͣλ1λ����żУ��λ
*/
static vu8 receive_flag = 0;
static vu8 receive_data[128] ={0};
vu16 size = 0;
void Inital_Uart1(void)
{
	RCC_APB2ENR |= (1<<2 )| (1<<14); 	//ʹ��GPIOA��UART1ģ��
	/*��ʼ������Ϊ���������������������Ϊ���գ�û�и�������ʹ����ͨ����������)*/
	GPIO_SET(GPIOA,1<<9,2,2,0);//�����������
	GPIO_SET(GPIOA,1<<10,2,1,0);//��������
	UART1->CR1 |= 1<<13;//ʹ��uart UEλ
	//Ĭ��û��żУ��λ
	//Ĭ���ֳ�Ϊ8���ֽڣ���Ϊû����żУ��λ������data bit 8λ 
	//Ĭ��ֹͣλΪ1λ
	
	uint32_t div_fraction = 0xA;
	uint32_t div_mantissa = 0x27;
	UART1->BRR = 0;//��λ
	UART1->BRR |= div_fraction | (div_mantissa<<4);//���ò�����Ϊ115200
	
	Inital_Recieve_Uart_IRQ();//ʹ��RXNE Interrupt
	
}

void Send_Byte(uint8_t byte)
{
	while((UART1->SR & (1<<7)) == 0); //�ȴ�SR��TXEλΪ1������TDR����Ϊ��
									//ÿ��дTDR������ʱ��SR��TXE���ᱻreset
	UART1->DR = byte;
}
//UART1���жϷ�����
void USART1_IRQHandler(void)
{
	receive_data[size] = UART1->DR;
	size++;
}

void Send_Data(uint8_t* data,uint16_t size)
{
	if(size == 0) //����ֹsize ==0 ʱ����41������
		return;
	UART1->CR1 |= 1<<3;//ʹ��TX ������һ��idle frame
	uint16_t i =0;
	for(;i<size;i++)
	{
		Send_Byte(*(data+i));
	}
	while ((UART1->SR & (1<<6)) == 0);//�ȴ��������ݷ������
	 UART1->SR = ~(1<<6);//��RCΪ��Ϊ0
}
uint8_t Receive_Byte(uint8_t *byte)
{
	uint16_t i = 0;
	while((UART1->SR&(1<<5)) == 0 && i<10000)//�ȴ������ݽ��գ���������λ�Ĵ������ݱ�ת�Ƶ�RDRʱ��SR��RXNE�ᱻset
		i++;
	if(i == 10000 ) //i = 10000˵��û�����ݽ�����
		return 1;
	*byte = (uint8_t)UART1->DR; //ÿ�ζ�ȡRDR��SR��RXNEλ�ᱻreset
	return 0;
}

uint16_t Receive_Data(uint8_t *data)
{
	uint8_t * p =data;
	uint16_t size = 0;
	
	UART1->CR1 |= 1<<2;//ʹ��RX
	while(Receive_Byte(p) ==0)
	{
		p++;
		size++;
	}
	return size;
}

void Inital_Recieve_Uart_IRQ(void)
{
	UART1->CR1 |= 1<<2;//ʹ��RX
	UART1->CR1 |= 1<<5;//ʹ��RXNE interrupt
}

uint16_t Get_Size()
{
	return size;
}
void Set_Size(vu16 set_size)
{
	size = set_size;
}

vu8 * Get_Receive_Data()
{
	return receive_data;
}

int fputc(int ch,FILE *fp)
{
	Send_Data((unsigned char *)&ch,1);
	return ch;
}

