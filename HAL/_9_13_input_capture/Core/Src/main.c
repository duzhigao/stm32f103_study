/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define TR_RISING 0
#define TR_FALLING 1
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t TIM4CH1_CAPTURE_STA = 0; //输入捕获状态 bit0~bit5表示溢出次数  bit6表示上升沿已捕获 bit7表示下降沿已捕获
uint32_t TIM4CH1_CAPTURE_VAL = 0; //输入捕获值
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t OverFlowCount;//溢出次数
  long long HighLevelTime = 0; //高电平脉冲多少个
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_Init();
  /* USER CODE BEGIN 2 */
  __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);//使能update中断，set DIER的UIE位
  HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);//使能捕获比较中断，使能捕获比较通道，并使能计数器，set DIER的CC1IE位，set CCER的CC1E位 ，set CR1的CEN位
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);//使能捕获比较通道，并使能计数器，set CCER的CC1E位，set CR1的CEN位
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(TIM4CH1_CAPTURE_STA&0X80) //bit7=1 说明成功捕获到了一次高电平
		{
			OverFlowCount = TIM4CH1_CAPTURE_STA&0X3F;
			HighLevelTime = OverFlowCount;
			printf("OverFlowCount = %d\r\n", OverFlowCount);
			/*溢出时间总和*/
			HighLevelTime = (HighLevelTime*(htim4.Init.Period+1))+TIM4CH1_CAPTURE_VAL;
			printf("HIGH:%lld\r\n",HighLevelTime);//打印总的高点平计数次数
			TIM4CH1_CAPTURE_STA=0; //一次捕获完成，重置 0
		}
	  
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*捕获中断回调函数*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim4)
	{
		if((TIM4CH1_CAPTURE_STA & 0x80) == 0) //bit7 为 0 说明一次捕获没有完成
		{
			/*bit6=1 说明刚才已经捕获到上升沿，本次中断由下降沿触发导致*/
			if(TIM4CH1_CAPTURE_STA & 0x40) //bit6==1，表示上升沿已捕获，由下降沿触发中断
			{
				//printf("CNT:%d\r\n",__HAL_TIM_GetCounter(&htim4));
				TIM4CH1_CAPTURE_STA |= 0x80; //下降沿到说明一次捕获结束
				TIM4CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_1);//读取捕获比较寄存器CCR1的值
				
				/*设置为上升沿触发*/
				TIM_RESET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1);//reset CCER的CC1P位和CC1NE位
				TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);//将CCER的CC1P位设置为上升沿触发
			}
			else //bit6 不等于 1，说明本次中断由上升沿触发
			{
				__HAL_TIM_DISABLE(&htim4);//停止计数，resetCR1的CEN位
				TIM4CH1_CAPTURE_STA = 0;
				TIM4CH1_CAPTURE_STA |= 0x40; //bit6 置 1 说明上升沿捕获到了
				TIM4CH1_CAPTURE_VAL = 0;
				__HAL_TIM_SetCounter(&htim4, 0);//设置CNT寄存器的值
				
				/*设置为下降沿触发*/
				TIM_RESET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1);//reset CCER的CC1P位和CC1NE位
				TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);//将CCER的CC1P位设置为下降沿触发
				
				__HAL_TIM_ENABLE(&htim4);//开始计数，setCR1的CEN位
			}
		}
	}
}

/*溢出中断回调函数*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim4)
	{
		if((TIM4CH1_CAPTURE_STA & 0x80) == 0) //结果为 0 说明一次捕获没有完成
		{
			if(TIM4CH1_CAPTURE_STA & 0x40) //bit6=1 说明捕获到上升沿
			{
				if((TIM4CH1_CAPTURE_STA & 0x3F) == 0x3F) //判断计数位满没有
				{
					TIM4CH1_CAPTURE_STA |= 0x80; //满，说明高电平太长，可能出错了
					TIM4CH1_CAPTURE_STA &= ~0x3F;
					TIM4CH1_CAPTURE_VAL = 0xFFFFFFFF;
				}
				else
				{
					TIM4CH1_CAPTURE_STA++; //如果低 6 位还没有满，溢出一次加一次
					__HAL_TIM_SetCounter(&htim4,0);//这里将CNT设为0，是因为下降沿中断时，由于速度太快了，CNT的值还没有来得及被硬件置为0，导致多加一个ARR【先是溢出中断，然后下降沿中断】
													//也可以把这代码注释，然后将TIM4的ARR的值增大一点比如+1
					//printf("---------DEBUG-----------\r\n");
					//printf("TIM4CH1_CAPTURE_STA:%d--------",TIM4CH1_CAPTURE_STA);
				}
	
			}
		}
	}
	
}

int32_t fputc(int32_t ch, FILE *f)
{
	uint8_t temp = (uint8_t)ch;
	HAL_USART_Transmit(&husart1, &temp, 1, 10);
	return ch;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
