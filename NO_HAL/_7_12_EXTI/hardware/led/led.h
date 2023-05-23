#ifndef _LED_H_
#define _LED_H_

#include "stm32f103zet6.h"
#include "typedef.h"

#define LED_RED BA((uint32_t)&(GPIOB->ODR),0)
#define LED_BLUE BA((uint32_t)&(GPIOB->ODR),5)

#define ON 0 
#define OFF 1

void LED_Init(void);

#endif