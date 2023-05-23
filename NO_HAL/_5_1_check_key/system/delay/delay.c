#include "delay.h"
#include "typedef.h"
#include "sys.h"
/*延时函数定义*/ 
void Delay(uint16_t ms)
{
	uint16_t i = ms / 500;
	uint16_t j = ms % 500;
	while(i>0)
	{
		Delay_Tick(500);
		i--;
	}
	Delay_Tick(j);
}