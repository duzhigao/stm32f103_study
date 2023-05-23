#ifndef _SYS_H_
#define _SYS_H_
#include "typedef.h"

void GPIO_SET(GPIO_TypeDef* GPIOX,uint16_t pin,uint8_t mode,uint8_t mode_cnf,uint8_t updwn);
void System_Clock_Init(void);
void System_Clock_Set(void);
void Delay_Tick(uint16_t ms);
void NVIC_PriorityGroupConfig(uint8_t group);
void NVIC_Config(uint8_t Channel,uint8_t Group,uint8_t PreemptionPriority,uint8_t SubPriority);

#endif