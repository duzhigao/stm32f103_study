#include "sys.h"
#include "tim.h"
int main(void)
{
	System_Clock_Init();
	TIM3_PWM_Init();
	TIM_PWM_Start();
	while(1);
	
}