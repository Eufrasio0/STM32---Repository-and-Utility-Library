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
#include "AtvvAvaliativaOne.h"
#include "BibUSTM32.h"
#include "stdio.h"

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

#include <stdint.h>
#define SINE_SAMPLES 100



void aula09_10(){
	DAC_Init1(DAC_CHANNEL2);


	const uint16_t senoide[SINE_SAMPLES] = {
			  2048, 2176, 2304, 2431, 2557, 2681, 2802, 2921, 3037, 3149,
			    3257, 3361, 3460, 3554, 3643, 3726, 3803, 3874, 3938, 3995,
			    4045, 4087, 4095, 4087, 4045, 3995, 3938, 3874, 3803, 3726,
			    3643, 3554, 3460, 3361, 3257, 3149, 3037, 2921, 2802, 2681,
			    2557, 2431, 2304, 2176, 2048, 1920, 1792, 1665, 1539, 1415,
			    1294, 1175, 1059, 947, 839, 735, 636, 542, 453, 370,
			    293, 222, 158, 101, 51, 9, 0, 9, 51, 101,
			    158, 222, 293, 370, 453, 542, 636, 735, 839, 947,
			    1059, 1175, 1294, 1415, 1539, 1665, 1792, 1920, 2048, 2176,
			    2304, 2431, 2557, 2681, 2802, 2921, 3037, 3149, 3257, 3361
	};

	for(int i = 0; i < SINE_SAMPLES; i++){
		DAC_SetValue(DAC_CHANNEL2, senoide[i], DAC_RES_12BITS);
		Delay_us(20);
	}

}

void teste_compiller(){
	  GPIO_Clock_Enable(GPIOA);
	  GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);

	  while (1)
	  {
	      GPIO_Toggle_Pin(GPIOA, PIN_6);
	      Delay_ms(1000);
	  }
}

void aula16_10(){

    ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
    USART1_Init();

    ADC_SingleChannel(ADC1, ADC_IN0);

    uint16_t media[5];

    while(1){
        float total = 0;
        for(int i = 0; i < 5; i++){
            media[i] = ADC_GetSingleConversion(ADC1);
            Delay_ms(2);
        }
        for(int i = 0; i < 5; i++){
            total += media[i];
        }
        float media_bits = total / 5.0;
        float tensao_media = (3.3 * media_bits) / 4095.0;
        printf("Valor medio lido: %.2f V\n", tensao_media);

        Delay_ms(200);
    }
}

int distanciaa(int Trigger, int Echo)
{
    GPIO_Write_Pin(GPIOA, Trigger, LOW);
    Delay_us(2);

    GPIO_Write_Pin(GPIOA, Trigger, HIGH);
    Delay_us(10);
    GPIO_Write_Pin(GPIOA, Trigger, LOW);

    uint32_t timeout = 30000;

    // Espera o Echo ficar HIGH
    while (!GPIO_Read_Pin(GPIOA, Echo) && timeout > 0)
    {
        Delay_us(1);
        timeout--;
    }

    if (timeout == 0)
    {
        printf("ERRO: Echo nunca ficou HIGH\r\n");
        return -1;
    }

    printf("Echo detectado!\r\n");

    timeout = 30000;

    // Espera o Echo voltar para LOW
    printf("Echo detectado!\r\n");

    Delay_ms(100);

    if (GPIO_Read_Pin(GPIOA, Echo))
    {
        printf("PA1 continua HIGH apos 100 ms\r\n");
    }
    else
    {
        printf("PA1 voltou para LOW\r\n");
    }

    return 0;

    if (timeout == 0)
    {
        printf("ERRO: Echo permaneceu HIGH\r\n");
        return -1;
    }

    printf("Echo voltou para LOW\r\n");

    return 0;
}

void testarEcho(void)
{
    GPIO_Clock_Enable(GPIOA);
    GPIO_Pin_Mode(GPIOA, PIN_1, INPUT);

    USART1_Init();

    while (1)
    {
        printf("PA1 = %d\r\n",
               GPIO_Read_Pin(GPIOA, PIN_1));

        Delay_ms(500);
    }
}

void rotinasensor(void)
{
    GPIO_Clock_Enable(GPIOA);

    GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
    GPIO_Pin_Mode(GPIOA, PIN_1, INPUT);

    USART1_Init();

    while (1)
    {
        int dist = distanciaa(PIN_0, PIN_1);

        if (dist >= 0)
            printf("Distancia: %d cm\n", dist);
        else
            printf("Erro na leitura do sensor\n");

        Delay_ms(1000);
    }
}

void testarSensor(void)
{
    GPIO_Clock_Enable(GPIOA);

    GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
    GPIO_Pin_Mode(GPIOA, PIN_1, INPUT);

    USART1_Init();

    while (1)
    {
        GPIO_Write_Pin(GPIOA, PIN_0, LOW);
        Delay_ms(100);

        printf("PA1 antes do trigger: %d\r\n",
               GPIO_Read_Pin(GPIOA, PIN_1));

        GPIO_Write_Pin(GPIOA, PIN_0, HIGH);
        Delay_us(10);
        GPIO_Write_Pin(GPIOA, PIN_0, LOW);

        Delay_ms(50);

        printf("PA1 depois do trigger: %d\r\n",
               GPIO_Read_Pin(GPIOA, PIN_1));

        Delay_ms(1000);
    }
}



void aula21_10(void){

    GPIO_Clock_Enable(GPIOA);
    GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
    ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
    USART1_Init();

    ADC_SingleChannel(ADC1, ADC_IN0);

    uint16_t media[15];
    int angulo = 0;

    while(1){

        float total = 0;

        for(int i = 0; i < 15; i++){
            media[i] = ADC_GetSingleConversion(ADC1);
        }

        for(int i = 0; i < 15; i++){
            total += media[i];
        }

        float media_bits = total / 15.0f;
        float tensao_media = (3.3f * media_bits) / 4095.0f;

        if(tensao_media > 3.0f && angulo < 180){
            angulo += 30;
            if(angulo > 180) angulo = 180;
        }
        else if(tensao_media < 1.0f && angulo > 0){
            angulo -= 30;
            if(angulo < 0) angulo = 0;
        }
        int pulso = 500 + (angulo * 2000) / 180;

            GPIO_Write_Pin(GPIOA, PIN_1, HIGH);
            Delay_us(pulso);

            GPIO_Write_Pin(GPIOA, PIN_1, LOW);
            Delay_us(20000 - pulso);


        printf("{%.2f V", tensao_media);
        printf("---", tensao_media);

        printf("%d  graus ]\n", angulo);

        Delay_ms(20);


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
	/* As funções referentes a atividade avaliativa estao dentro do header AtvvAvaliaticaOne.h, nela estão as funções*/

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

  /* Chame aqui a função referente a rotina */

  aula21_10();
  /* USER CODE END 2
   * Dó (C4): 261,63 HzRé (D4): 293,66 HzMi (E4): 329,63 HzFá (F4): 349,23 HzSol (G4): 392,00 HzLá (A4): 440,00 HzSi (B4): 493,88 Hz*/

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
