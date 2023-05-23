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
uint8_t TIM4CH1_CAPTURE_STA = 0; //���벶��״̬ bit0~bit5��ʾ�������  bit6��ʾ�������Ѳ��� bit7��ʾ�½����Ѳ���
uint32_t TIM4CH1_CAPTURE_VAL = 0; //���벶��ֵ
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
	uint8_t OverFlowCount;//�������
  long long HighLevelTime = 0; //�ߵ�ƽ������ٸ�
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
  __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);//ʹ��update�жϣ�set DIER��UIEλ
  HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);//ʹ�ܲ���Ƚ��жϣ�ʹ�ܲ���Ƚ�ͨ������ʹ�ܼ�������set DIER��CC1IEλ��set CCER��CC1Eλ ��set CR1��CENλ
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);//ʹ�ܲ���Ƚ�ͨ������ʹ�ܼ�������set CCER��CC1Eλ��set CR1��CENλ
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(TIM4CH1_CAPTURE_STA&0X80) //bit7=1 ˵���ɹ�������һ�θߵ�ƽ
		{
			OverFlowCount = TIM4CH1_CAPTURE_STA&0X3F;
			HighLevelTime = OverFlowCount;
			printf("OverFlowCount = %d\r\n", OverFlowCount);
			/*���ʱ���ܺ�*/
			HighLevelTime = (HighLevelTime*(htim4.Init.Period+1))+TIM4CH1_CAPTURE_VAL;
			printf("HIGH:%lld\r\n",HighLevelTime);//��ӡ�ܵĸߵ�ƽ��������
			TIM4CH1_CAPTURE_STA=0; //һ�β�����ɣ����� 0
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
/*�����жϻص�����*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim4)
	{
		if((TIM4CH1_CAPTURE_STA & 0x80) == 0) //bit7 Ϊ 0 ˵��һ�β���û�����
		{
			/*bit6=1 ˵���ղ��Ѿ����������أ������ж����½��ش�������*/
			if(TIM4CH1_CAPTURE_STA & 0x40) //bit6==1����ʾ�������Ѳ������½��ش����ж�
			{
				//printf("CNT:%d\r\n",__HAL_TIM_GetCounter(&htim4));
				TIM4CH1_CAPTURE_STA |= 0x80; //�½��ص�˵��һ�β������
				TIM4CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_1);//��ȡ����ȽϼĴ���CCR1��ֵ
				
				/*����Ϊ�����ش���*/
				TIM_RESET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1);//reset CCER��CC1Pλ��CC1NEλ
				TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);//��CCER��CC1Pλ����Ϊ�����ش���
			}
			else //bit6 ������ 1��˵�������ж��������ش���
			{
				__HAL_TIM_DISABLE(&htim4);//ֹͣ������resetCR1��CENλ
				TIM4CH1_CAPTURE_STA = 0;
				TIM4CH1_CAPTURE_STA |= 0x40; //bit6 �� 1 ˵�������ز�����
				TIM4CH1_CAPTURE_VAL = 0;
				__HAL_TIM_SetCounter(&htim4, 0);//����CNT�Ĵ�����ֵ
				
				/*����Ϊ�½��ش���*/
				TIM_RESET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1);//reset CCER��CC1Pλ��CC1NEλ
				TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);//��CCER��CC1Pλ����Ϊ�½��ش���
				
				__HAL_TIM_ENABLE(&htim4);//��ʼ������setCR1��CENλ
			}
		}
	}
}

/*����жϻص�����*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim4)
	{
		if((TIM4CH1_CAPTURE_STA & 0x80) == 0) //���Ϊ 0 ˵��һ�β���û�����
		{
			if(TIM4CH1_CAPTURE_STA & 0x40) //bit6=1 ˵������������
			{
				if((TIM4CH1_CAPTURE_STA & 0x3F) == 0x3F) //�жϼ���λ��û��
				{
					TIM4CH1_CAPTURE_STA |= 0x80; //����˵���ߵ�ƽ̫�������ܳ�����
					TIM4CH1_CAPTURE_STA &= ~0x3F;
					TIM4CH1_CAPTURE_VAL = 0xFFFFFFFF;
				}
				else
				{
					TIM4CH1_CAPTURE_STA++; //����� 6 λ��û���������һ�μ�һ��
					__HAL_TIM_SetCounter(&htim4,0);//���ｫCNT��Ϊ0������Ϊ�½����ж�ʱ�������ٶ�̫���ˣ�CNT��ֵ��û�����ü���Ӳ����Ϊ0�����¶��һ��ARR����������жϣ�Ȼ���½����жϡ�
													//Ҳ���԰������ע�ͣ�Ȼ��TIM4��ARR��ֵ����һ�����+1
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
