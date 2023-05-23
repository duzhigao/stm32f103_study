#ifndef _TIME_H_
#define _TIME_H_

#define TIM3 ((TIM_TypeDef *)0x40000400)
#include "typedef.h"
typedef struct
{
  vu32 CR1;             /*!< TIM control register 1,                      Address offset: 0x00 */
  vu32 CR2;             /*!< TIM control register 2,                      Address offset: 0x04 */
  vu32 SMCR;            /*!< TIM slave Mode Control register,             Address offset: 0x08 */
  vu32 DIER;            /*!< TIM DMA/interrupt enable register,           Address offset: 0x0C */
  vu32 SR;              /*!< TIM status register,                         Address offset: 0x10 */
  vu32 EGR;             /*!< TIM event generation register,               Address offset: 0x14 */
  vu32 CCMR1;           /*!< TIM  capture/compare mode register 1,        Address offset: 0x18 */
  vu32 CCMR2;           /*!< TIM  capture/compare mode register 2,        Address offset: 0x1C */
  vu32 CCER;            /*!< TIM capture/compare enable register,         Address offset: 0x20 */
  vu32 CNT;             /*!< TIM counter register,                        Address offset: 0x24 */
  vu32 PSC;             /*!< TIM prescaler register,                      Address offset: 0x28 */
  vu32 ARR;             /*!< TIM auto-reload register,                    Address offset: 0x2C */
  vu32 RCR;             /*!< TIM  repetition counter register,            Address offset: 0x30 */
  vu32 CCR1;            /*!< TIM capture/compare register 1,              Address offset: 0x34 */
  vu32 CCR2;            /*!< TIM capture/compare register 2,              Address offset: 0x38 */
  vu32 CCR3;            /*!< TIM capture/compare register 3,              Address offset: 0x3C */
  vu32 CCR4;            /*!< TIM capture/compare register 4,              Address offset: 0x40 */
  vu32 BDTR;            /*!< TIM break and dead-time register,            Address offset: 0x44 */
  vu32 DCR;             /*!< TIM DMA control register,                    Address offset: 0x48 */
  vu32 DMAR;            /*!< TIM DMA address for full transfer register,  Address offset: 0x4C */
  vu32 OR;              /*!< TIM option register,                         Address offset: 0x50 */
  
}TIM_TypeDef;

void TIM3_PWM_Init(void);
void TIM_PWM_Start();

#endif