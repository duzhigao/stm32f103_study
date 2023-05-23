#ifndef _CORTEX_M3_H
#define _CORTEX_M3_H
#include "typedef.h"

#define ISER ((vu32 *)0xE000E100)  //Interrupt Set Enable Register
#define IP ((vu8 *)0xE000E400)		//Interrupt Prioty Register
#define AIRCR ((vu32 *)0xE000ED0C)	//Application Interrupt Reset Control Register
void NVIC_Config(uint8_t Channel,uint8_t Group,uint8_t PreemptionPriority,uint8_t SubPriority);
void NVIC_PriorityGroupConfig(uint8_t group);

#endif