#include "key.h"
#include "led.h"
#include "stm32f1xx.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_Delay(10);
	if(KEY1 == 0)
	{
		LED_RED_OFF();
		LED_BLUE_ON();
		HAL_Delay(5000);
		LED_BLUE_OFF();
	}
	
}