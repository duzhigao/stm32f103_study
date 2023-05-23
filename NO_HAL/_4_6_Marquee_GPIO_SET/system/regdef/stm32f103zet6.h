#ifndef _STM32F103ZET6_H_
#define _STM32F103ZET6_H_
#include "typedef.h"
#define RCC_CR (*((vu32 *)0x40021000))
#define RCC_CFGR (*((vu32 *)0x40021004))
#define RCC_CIR (*((uint32_t *)(0x40021000+0x08)))
#define RCC_APB2ENR (*((vu32 *)0x40021018))
#define GPIOB ((GPIO_TypeDef *) 0x40010C00)
#define SYS_STICK ((SysTick_Type *)0xE000E010)
	
#define FLASH_ACR (*((uint32_t *)(0x40022000)))

#endif