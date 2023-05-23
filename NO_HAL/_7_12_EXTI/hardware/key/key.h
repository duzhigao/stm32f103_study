#ifndef _KEY_H_
#define _KEY_H_

#include "typedef.h"
#include "stm32f103zet6.h"

#define KEY1 BA((uint32_t)(&GPIOA->IDR),0)
#define KEY2 BA((uint32_t)&(GPIOG->IDR),15)
#define KEY3 BA((uint32_t)(uint32_t)&(GPIOC->IDR),13)
#define KEY4 BA((uint32_t)&(GPIOE->IDR),3)

#define PRESSED 0
#define NO_PRESSED 1
//key1-PA0
//key2-PG15
//key3-PC13
//key4-PE3
uint8_t Scan_Key(void);
void KEY_Init(void);


#endif
