#ifndef _STM32F103ZET6_H_
#define _STM32F103ZET6_H_
#include "typedef.h"
#define RCC_CR (*((vu32 *)0x40021000))
#define RCC_CFGR (*((vu32 *)0x40021004))
#define RCC_CIR (*((vu32 *)(0x40021000+0x08)))
#define RCC_APB2ENR (*((vu32 *)0x40021018))
#define RCC_APB1ENR (*((vu32 *)0x4002101C))

#define GPIOA ((GPIO_TypeDef *) 0x40010800)
#define GPIOB ((GPIO_TypeDef *) 0x40010C00)
#define GPIOC ((GPIO_TypeDef *) 0x40011000)
#define GPIOD ((GPIO_TypeDef *) 0x40011400)
#define GPIOE ((GPIO_TypeDef *) 0x40011800)
#define GPIOF ((GPIO_TypeDef *) 0x40011C00)
#define GPIOG ((GPIO_TypeDef *) 0x40012000)


#define SYS_STICK ((SysTick_Type *)0xE000E010)
	
#define FLASH_ACR (*((vu32 *)(0x40022000)))


#define A(address,n) (((address&0xF0000000)+0x2000000)+((address&0xFFFFF)<<5)+(n<<2))  //�����Ӧλ�α��������洢��Ԫ�ĵ�ַ
#define BA(address,n) (*((vu32 *)(A(address,n))))	 //���ʶ�Ӧ�������Ĵ洢��Ԫ

#define UART1 ((UART_Typedef *)0x40013800)

#define EXTI_PR (*((vu32 *)0x40010414))

typedef struct
{
	vu32 SR;	//status register
	vu32 DR;	//data register
	vu32 BRR;	//baurd rate register
	vu32 CR1;	//control register 1
	vu32 CR2;	//control register 2
	vu32 CR3;
	vu32 GTPR;
}UART_Typedef;
#endif