#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32f103zet6.h"
#include "typedef.h"

#define BEEP BA((uint32_t)&(GPIOA->ODR),8)


#define BEEP_ON 1 
#define BEEP_OFF 0

void Beep_Init(void);

#endif