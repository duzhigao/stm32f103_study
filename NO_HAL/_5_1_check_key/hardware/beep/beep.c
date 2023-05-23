#include "beep.h"
#include "sys.h"

void Beep_Init(void)
{
	RCC_APB2ENR |= 1<<3;
	GPIO_SET(GPIOA,1<<8,2,0,0);//ÍÆÍìÊä³ö
	BEEP = BEEP_OFF;
}