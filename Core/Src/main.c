/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t ADC_Value;
uint16_t ADC1_Value, ADC2_Value, ADC3_Value;
uint16_t count, x;
char message[100];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void Stepper_Forward(uint16_t n);
void Stepper_Backward(uint16_t n);
void Servo_Forward(uint16_t C,uint16_t T);
void Servo_Backward(uint16_t C,uint16_t T);
uint16_t ADC_Read(uint16_t C);

void Trans( uint16_t ADC1_Value, uint16_t ADC2_Value, uint16_t ADC3_Value);

void Bottom_Open(uint16_t T);
void Bottom_Close(uint16_t T);

void Top_Open(uint16_t T);
void Top_Close(uint16_t T);

void Screw_Up(uint16_t T);
void Screw_Down(uint16_t T);

void Robot_Up_Step(uint16_t S);
void Joystick_Contorl(void);




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
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_TIM1_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  
  HAL_ADC_Start(&hadc1);
  HAL_ADC_Start(&hadc2);
  HAL_ADC_Start(&hadc3);
  
  HAL_GPIO_WritePin(GPIOE, EN_Pin, GPIO_PIN_RESET);
  

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  // init
  
  
 Bottom_Open(300);
 Top_Open(300);
 
 HAL_Delay(5000);
 
 // Ask The Gripper Close
 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 170);
 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 130);
 

 // init over
 
 Robot_Up_Step(51);
  
   /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_TIM1
                              |RCC_PERIPHCLK_ADC12|RCC_PERIPHCLK_ADC34;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.Adc34ClockSelection = RCC_ADC34PLLCLK_DIV1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief  This function is for stepper forward
  * @param  $ n $ the number of loop #uint16_t
  * @retval None
  */

void Stepper_Forward(uint16_t n) 
{
	   HAL_GPIO_WritePin(GPIOA, DIR_Pin,GPIO_PIN_RESET);
	   HAL_Delay(1);
		  
		for(count = 0; count < n; count++){
			HAL_GPIO_WritePin(GPIOA, STP_Pin, GPIO_PIN_SET);
			HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOA, STP_Pin, GPIO_PIN_RESET);
			HAL_Delay(1);
		}
}

/**
  * @brief  This function is for stepper backword
  * @param  $ n $ is the number of loop #uint16_t
  * @retval None
  */


void Stepper_Backward(uint16_t n) 
{
	   HAL_GPIO_WritePin(GPIOA, DIR_Pin,GPIO_PIN_SET);
	   HAL_Delay(1);
		  
		for(count = 0; count < n; count++){
			HAL_GPIO_WritePin(GPIOA, STP_Pin, GPIO_PIN_SET);
			HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOA, STP_Pin, GPIO_PIN_RESET);
			HAL_Delay(1);
		}
}


/**
  * @brief  This function is for messaage sending
  * @param  $ C $ the channel of tim1 && $ T $ is the time for delay #uint16_t
  * @retval None
  */

void Trans( uint16_t ADC1_Value, uint16_t ADC2_Value, uint16_t ADC3_Value)
{
	snprintf(message, sizeof(message), "ADC_1 is: %d, ADC_2 is: %d, ADC_3 is: %d\n", ADC1_Value, ADC2_Value, ADC3_Value);
	HAL_UART_Transmit(&huart4,(uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}


/**
  * @brief  This function is for bottom gripper opening
  * @param  $ T $ is the time for servo rotating #uint16_t
  * @retval None
  */

void Bottom_Open(uint16_t T)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 130);
	HAL_Delay(T);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 0);
}

/**
  * @brief  This function is for bottom gripper closing
  * @param  $ T $ is the time for servo rotating #uint16_t
  * @retval None
  */
void Bottom_Close(uint16_t T)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 170);
	HAL_Delay(T);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 0);
}

/**
  * @brief  This function is for top gripper opening
  * @param  $ T $ is the time for servo rotating #uint16_t
  * @retval None
  */
void Top_Open(uint16_t T)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 170);
	HAL_Delay(T);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);
}

/**
  * @brief  This function is for top gripper closing
  * @param  $ T $ is the time for servo rotating #uint16_t
  * @retval None
  */
void Top_Close(uint16_t T)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 130);
	HAL_Delay(T);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);
}

/**
  * @brief  This function is for screw rotating, bottom upward
  * @param  $ T $ is the time for servo rotating #uint16_t
  * @retval None
  */
void Screw_Up(uint16_t T)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4, 170);
	HAL_Delay(T);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4, 0);
}

/**
  * @brief  This function is for screw rotating, bottom downward
  * @param  $ T $ is the time for servo rotating #uint16_t
  * @retval None
  */
void Screw_Down(uint16_t T)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4, 130);
	HAL_Delay(T);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4, 0);
}

/**
  * @brief  This function is for robot moving upward 
  * @param  $ S $ is the step upward
  * @retval None
  */
void Robot_Up_Step(uint16_t S)
{
	for(int i =0; i < S; i++){
		
		 HAL_Delay(5000);
 
		 Top_Open(300);
		 
		 HAL_Delay(2000);
		 
		 
		 Screw_Down(35000);
		 
		 //top step over
		 
		 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 170);
		 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 130);
		 
		  HAL_Delay(3000);
		 
		 Bottom_Open(300);
		 
		 Screw_Up(35000);
		 
		 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 170);
		 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 130);
			
	}
}


/**
  * @brief  This function is for read value from joystick
  * @param  $ C $ the channel of ADC #uint16_t
  * @retval $ ADC_Value $ the read value of ADC #uint16_t
  */

uint16_t ADC_Read(uint16_t C)
{
	if(C == 1)
	{	
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,1000);
		ADC_Value = HAL_ADC_GetValue(&hadc1);
		
	}else if(C == 2)
	{
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2,1000);
		ADC_Value = HAL_ADC_GetValue(&hadc2);
		
	}else if(C == 3)
	{
		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3,1000);
		ADC_Value = HAL_ADC_GetValue(&hadc3);
	}
	
	return ADC_Value;
}


/**
  * @brief  This function is for joystick control
  * @param  None
  * @retval None
  */

void Joystick_Contorl(void)
{
	
	  ADC1_Value = ADC_Read(1);
	  ADC2_Value = ADC_Read(2);
	  ADC3_Value = ADC_Read(3);
	  
	  
	  Trans(ADC1_Value, ADC2_Value, ADC3_Value);
	
	  if(ADC1_Value > 3800){
		  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 130);
	  }else if(ADC1_Value<1000){
		  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 170);
	  }else{
		  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);

	  }

	  if(ADC2_Value > 3800){
	  		  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 130);
	  	  }else if(ADC2_Value<1000){
	  		  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 170);
	  	  }else{
	  		  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 0);

	  }
		  
	  if(ADC3_Value > 3500){
		  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4, 130);

	  }else if(ADC3_Value<1000){
		  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4, 170);		  
	  }else{
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4, 0);

	  }
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
