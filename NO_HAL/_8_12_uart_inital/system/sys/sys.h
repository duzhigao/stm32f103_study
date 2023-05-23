#ifndef _SYS_H_
#define _SYS_H_
#include "typedef.h"
void GPIO_SET(GPIO_TypeDef* GPIOX,uint16_t pin,uint8_t mode,uint8_t mode_cnf,uint8_t updwn);
void System_Clock_Init(void);
void System_Clock_Set(void);
void Delay_Tick(uint16_t ms);
#endif