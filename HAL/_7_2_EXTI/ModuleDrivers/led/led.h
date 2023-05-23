#ifndef _LED_H
#define _LED_H
#include "stm32f1xx.h"
#define LED_RED_OFF() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1)
#define LED_RED_ON() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0)
#define LED_BLUE_OFF() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1)
#define LED_BLUE_ON() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0)

#endif