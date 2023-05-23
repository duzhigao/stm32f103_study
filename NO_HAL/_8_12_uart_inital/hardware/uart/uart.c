#include "uart.h"
#include "sys.h"
#include "stm32f103zet6.h""
/*TX -- PA9
**RX -- PA10
*设置波特率为115200
*字长为8，停止位1位，奇偶校验位
*/
void Inital_Uart1(void)
{
	RCC_APB2ENR |= (1<<2 )| (1<<14); 	//使能GPIOA和UART1模块
	/*初始化引脚为复用推挽输出，输入引脚为悬空（没有复用输入使用普通输入来代替)*/
	GPIO_SET(GPIOA,1<<9,2,2,0);//复用推挽输出
	GPIO_SET(GPIOA,1<<10,2,1,0);//悬空输入
	UART1->CR1 |= 1<<13;//使能uart UE位
	//默认没奇偶校验位
	//默认字长为8个字节，因为没有奇偶校验位，所以data bit 8位 
	//默认停止位为1位
	
	uint32_t div_fraction = 0xA;
	uint32_t div_mantissa = 0x27;
	UART1->BRR = 0;//清位
	UART1->BRR |= div_fraction | (div_mantissa<<4);//设置波特率为115200
}

void Send_Byte(uint8_t byte)
{
	while((UART1->SR & (1<<7)) == 0); //等待SR的TXE位为1，表明TDR数据为空
									//每次写TDR的数据时，SR的TXE都会被reset
	UART1->DR = byte;
}
void Send_Data(uint8_t* data,uint16_t size)
{
	if(size == 0) //这句防止size ==0 时会在41行阻塞
		return;
	UART1->CR1 |= 1<<3;//使能TX 并发送一个idle frame
	uint16_t i =0;
	for(;i<size;i++)
	{
		Send_Byte(*(data+i));
	}
	while ((UART1->SR & (1<<6)) == 0);//等待所有数据发送完成
	 UART1->SR = ~(1<<6);//将RC为置为0
}
uint8_t Receive_Byte(uint8_t *byte)
{
	uint16_t i = 0;
	while((UART1->SR&(1<<5)) == 0 && i<10000)//等待有数据接收，当接受移位寄存器数据被转移到RDR时，SR的RXNE会被set
		i++;
	if(i == 10000 ) //i = 10000说明没有数据接受了
		return 1;
	*byte = (uint8_t)UART1->DR; //每次读取RDR，SR的RXNE位会被reset
	return 0;
}

uint16_t Receive_Data(uint8_t *data)
{
	uint8_t * p =data;
	uint16_t size = 0;
	
	UART1->CR1 |= 1<<2;//使能RX
	while(Receive_Byte(p) ==0)
	{
		p++;
		size++;
	}
	return size;
}