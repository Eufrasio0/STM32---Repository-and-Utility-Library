/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "Utility.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void aula_09_09(){

	Utility_Init();
	GPIO_Clock_Enable(GPIOE);
	GPIO_Clock_Enable(GPIOD);
	GPIO_Clock_Enable(GPIOA);


	GPIO_Pin_Mode(GPIOE, PIN_3, INPUT);
	GPIO_Pin_Mode(GPIOE, PIN_4, INPUT);
	GPIO_Resistor_Enable(GPIOE, PIN_3, PULL_UP);
	GPIO_Resistor_Enable(GPIOE, PIN_4, PULL_UP);

	GPIO_Pin_Mode(GPIOD, PIN_8, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);



	EXTI_Config(EXTI3, GPIOE, FALLING_EDGE);

	NVIC_EnableIRQ(EXTI3_IRQn);


	EXTI_Config(EXTI4, GPIOE, FALLING_EDGE);

	NVIC_EnableIRQ(EXTI4_IRQn);


}




void aula_11_09(){
	//Configura Clock
	GPIO_Clock_Enable(GPIOE);
	GPIO_Clock_Enable(GPIOA);

	//Define os botoes e habilita o resistor interno
	GPIO_Pin_Mode(GPIOE, PIN_3, INPUT);
	GPIO_Pin_Mode(GPIOE, PIN_4, INPUT);
	GPIO_Pin_Mode(GPIOA, PIN_0, INPUT);
	GPIO_Resistor_Enable(GPIOE, PIN_3, PULL_UP);
	GPIO_Resistor_Enable(GPIOE, PIN_4, PULL_UP);
	GPIO_Resistor_Enable(GPIOA, PIN_0, PULL_DOWN);

	//Define os botoes da placa
	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);
	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
	GPIO_Write_Pin(GPIOA, PIN_7, HIGH);


	//Definir interrupções

	EXTI_Config(EXTI3, GPIOE, FALLING_EDGE);
	EXTI_Config(EXTI4, GPIOE, FALLING_EDGE);
	EXTI_Config(EXTI0, GPIOA, RISING_EDGE);

	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);


	NVIC_SetPriority(EXTI3_IRQn, 0);
	NVIC_SetPriority(EXTI4_IRQn, -1);
	NVIC_SetPriority(EXTI0_IRQn, -2);


}

void rotina(){
	for(int i = 0; i < 5;i++){
		GPIO_Toggle_Pin(GPIOA, PIN_6);
		Delay_ms(500);
		GPIO_Toggle_Pin(GPIOA, PIN_6);
		Delay_ms(500);
	}
}

void rotina_secundaria(){
	for(int i = 0; i < 25;i++){
		GPIO_Toggle_Pin(GPIOA, PIN_7);
		Delay_ms(50);
		GPIO_Toggle_Pin(GPIOA, PIN_7);
		Delay_ms(50);
	}
}
int marcador = 0;

void mudar_prioridade(){
	if(marcador % 2 == 0){
		GPIO_Write_Pin(GPIOA, PIN_6, LOW);
		Delay_ms(100);
		GPIO_Write_Pin(GPIOA, PIN_6, HIGH);

		NVIC_SetPriority(EXTI3_IRQn, -1);
		NVIC_SetPriority(EXTI4_IRQn, 0);
	} else{
		GPIO_Write_Pin(GPIOA, PIN_7, LOW);
		Delay_ms(100);
		GPIO_Write_Pin(GPIOA, PIN_7, HIGH);

		NVIC_SetPriority(EXTI3_IRQn, 0);
		NVIC_SetPriority(EXTI4_IRQn, -1);
	}
	marcador++;
}


void EXTI3_IRQHandler(){
	rotina_secundaria();
	EXTI_Clear_Pending(EXTI3);
}

void EXTI4_IRQHandler(){
	rotina();
	EXTI_Clear_Pending(EXTI4);

}

void EXTI0_IRQHandler(){
	mudar_prioridade();
	EXTI_Clear_Pending(EXTI0);

}

void PWM_LED(int tempo){


	int marc = 0;

	while(1){

		GPIO_Write_Pin(GPIOA, PIN_6, LOW);
		Delay_us(tempo);
		GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
		Delay_us(10000 - tempo);
		if(marc == 0){
			tempo = tempo - 100;
		}
		if(marc == 1){
			tempo = tempo + 100;
		}

		if(tempo == 0){
			marc = 1;
		}
		if(tempo == 10000){
			marc = 0;
		}
	}
}



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
  Utility_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();


  aula_11_09();





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
