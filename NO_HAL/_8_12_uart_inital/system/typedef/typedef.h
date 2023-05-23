#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#define uint8_t unsigned char 
#define uint16_t unsigned short 
#define uint32_t unsigned int

#define vu32 volatile uint32_t
#define PIN(n) (1<<(n))
typedef struct
{
	vu32 CR[2];
	vu32 IDR;
	vu32 ODR;
	vu32 BSRR;
	vu32 BRR;
	vu32 LCKR;
}GPIO_TypeDef;

typedef struct
{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
}SysTick_Type;

#endif


