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
#include "stdint.h"
#include "voz_dac_8bits.h"



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

void aula09_10(){

	//pa4 = dac1
	//pa5 = dac2

	DAC_Init1(DAC_CHANNEL2);
	int SINE_SAMPLES = 100;

	const uint16_t senoide[] = {
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


void rotinasensor(void)
{
    GPIO_Clock_Enable(GPIOA);

    GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT); //trigger
    GPIO_Pin_Mode(GPIOA, PIN_1, INPUT); //echo

    USART1_Init();

    while (1)
    {
        int dist = getdistancia(PIN_0, PIN_1);

        if (dist >= 0)
            printf("Distancia: %d cm\r\n", dist);
        else
            printf("Erro na leitura do sensor\n");

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

void aula28_10(){

    ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
    GPIO_Clock_Enable(GPIOA);
    GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
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
        if(tensao_media > 2){
        	GPIO_Write_Pin(GPIOA, PIN_6, LOW);
        } else if(tensao_media < 2)
    	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
    }
}


void sensorinternotemp(){

	USART1_Init();

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC->CCR |= 0b01 << 16;
	ADC1->SQR1 &= ~(0xF << 20);
	ADC1->SQR3 |= 16;
	ADC1->SMPR1 |= (7 << 18);
	ADC->CCR |= ADC_CCR_TSVREFE;
	ADC1->CR2 |= ADC_CR2_ADON;

	uint32_t *p = (uint32_t *) 0x1FFF7A2C;
	uint32_t Word = *p;
	uint16_t TS_CAL1 = (Word & 0x0000FFFF);
	uint16_t TS_CAL2 = (Word & 0xFFFF0000) >> 16;

	while(1){

		ADC1->CR2 |= ADC_CR2_SWSTART;
		while(!(ADC1->SR & ADC_SR_EOC));
		float temperatura = ((80 * (float)(ADC1->DR - TS_CAL1)) / (TS_CAL2 - TS_CAL1)) + 30;
		printf("Temperatura do chip= %.2f\r\n", temperatura);
		Delay_ms(500);
	}

}


void aula04_11(void)
{
    ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);

    GPIO_Clock_Enable(GPIOA);
    GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);

    USART1_Init();

    ADC_SingleChannel(ADC1, ADC_IN0);

    uint16_t valores[25];
    uint32_t soma;
    float media;
    float temperatura;

    while(1)
    {
        soma = 0;
        for(int i = 0; i < 25; i++)
        {
            valores[i] = ADC_GetSingleConversion(ADC1);
        }
        for(int i = 0; i < 25; i++)
        {
            soma += valores[i];
        }
        media = (float)soma / 25.0f;
        temperatura = 25.0f + (media * 1575.0f / 4095.0f);

        printf("Temperatura = %.2f C\r\n", temperatura);

        Delay_ms(500);
    }
}


void aula04_11resistor(void)
{
    ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
    USART1_Init();
    ADC_SingleChannel(ADC1, ADC_IN0);
    uint16_t conversao;
    float lumens;
    float novo_valor;

    while(1)
    {
        conversao = ADC_GetSingleConversion(ADC1);
        lumens = 100.0f + ((float)conversao * 700.0f / 4095.0f);
        novo_valor = 1 - (lumens / 800);
        printf("Iluminancia = %.2f lux\r\n", novo_valor * 800);
        Delay_ms(1000);
    }
}


static inline float ADC_rotiner_escalonador(ADC_TypeDef *ADCx, int PIN, float minimo, float maximo){
    uint16_t conversao;
    conversao = ADC_GetSingleConversion(ADCx);
    float valor = minimo + ((float)conversao * (maximo - minimo) / 4095.0f);
    return valor;
}

void aula05_11(){
    DAC_Init1(DAC_CHANNEL2);

    ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
    ADC_Init(ADC2, SINGLE_CHANNEL, ADC_RES_12BITS);

    ADC_SingleChannel(ADC1, PIN_0);
    ADC_SingleChannel(ADC2, PIN_1);

    float potenciometro_one;
    float potenciometro_two;

    float fator_escala = 1.0f;

    uint32_t delay_amostra_us;

    const uint16_t samples[100]={
    		2048, 2176, 2304, 2431, 2557, 2680, 2801, 2919, 3034, 3145,
    		3251, 3353, 3449, 3540, 3625, 3704, 3776, 3842, 3900, 3951,
    		3995, 4031, 4059, 4079, 4091, 4095, 4091, 4079, 4059, 4031,
    		3995, 3951, 3900, 3842, 3776, 3704, 3625, 3540, 3449, 3353,
    		3251, 3145, 3034, 2919, 2801, 2680, 2557, 2431, 2304, 2176,
    		2048, 1919, 1791, 1664, 1538, 1415, 1294, 1176, 1061, 950,
    		844, 742, 646, 555, 470, 391, 319, 253, 195, 144,
    		100, 64, 36, 16, 4, 0, 4, 16, 36, 64,
    		100, 144, 195, 253, 319, 391, 470, 555, 646, 742,
    		844, 950, 1061, 1176, 1294, 1415, 1538, 1664, 1791, 1919};

    while(1){
        potenciometro_one = ADC_rotiner_escalonador(ADC1, PIN_0, 10.0f, 200.0f);
        potenciometro_two = ADC_rotiner_escalonador(ADC2, PIN_1, 0.0f, 3.3f);
        fator_escala = potenciometro_two / 3.3f;
        delay_amostra_us = (uint32_t)(10000.0f / potenciometro_one);

        for(int i = 0; i < 100; i++){

            uint16_t amostra_escalonada = (uint16_t)(samples[i] * fator_escala);

            DAC_SetValue(DAC_CHANNEL2,
                amostra_escalonada,
                DAC_RES_12BITS
            );

            Delay_us(delay_amostra_us);
        }
    }
}


void TIM3_Init(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 &= ~TIM_CR1_DIR;
	TIM3->PSC = 41999;
	TIM3->ARR = 40000;
	TIM3->CR2 = 0b010 << 4;
	TIM3->CR1 |= TIM_CR1_URS;
	TIM3->EGR = 1;
	TIM3->CR1 |= TIM_CR1_CEN;
}


void aula13_11(){
	GPIO_Clock_Enable(GPIOA);
	GPIO_Pin_Mode(GPIOA, PIN_7, ALTERNATE);
	GPIO_Alternate_Function(GPIOA, PIN_7, AF2);

    ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
    ADC_SingleChannel(ADC1, PIN_0);

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 &= ~TIM_CR1_DIR;
	TIM3->PSC = 419;
	TIM3->ARR = 99;
	TIM3->CCMR1 |= 0b110 << 12;
	TIM3->CCMR1 |= TIM_CCMR1_OC2PE;
	TIM3->CCER |= TIM_CCER_CC2E;
	TIM3->EGR = TIM_EGR_UG;
	TIM3->CR1 |= TIM_CR1_CEN;


	while(1){
		TIM3->CCR2 = (int)ADC_rotiner_escalonador(ADC1, PIN_0, -1.0f, 100.0f);
	}
}



void aula11_11(){
    GPIO_Clock_Enable(GPIOA);
    GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
    GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);

    SysTick->LOAD = 6.3e6 - 1;
    SysTick->CTRL = 0b011;
	while(1){
		GPIO_Toggle_Pin(GPIOA, PIN_6);
		Delay_ms(1000);
	}

}

void questao8prova2(){

    USART1_Init();

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC->CCR |= 0b01 << 16;
    ADC1->SQR1 &= ~(0xF << 20);
    ADC1->SQR3 |= 16;
    ADC1->SMPR1 |= (7 << 18);
    ADC->CCR |= ADC_CCR_TSVREFE;
    ADC1->CR2 |= ADC_CR2_ADON;

    uint32_t *p = (uint32_t *) 0x1FFF7A2C;
    uint32_t Word = *p;
    uint16_t TS_CAL1 = (Word & 0x0000FFFF);
    uint16_t TS_CAL2 = (Word & 0xFFFF0000) >> 16;
    uint32_t soma = 0;
    uint16_t media = 0;

    while(1){
        soma = 0;
        for(int i = 0; i < 50; i++){
            ADC1->CR2 |= ADC_CR2_SWSTART;
            while(!(ADC1->SR & ADC_SR_EOC));
            soma += ADC1->DR;
            Delay_ms(2);
        }
        media = soma / 50;
        float temperatura =
        ((80.0f * ((float)media - TS_CAL1)) /
        (TS_CAL2 - TS_CAL1)) + 30.0f;
        printf("Temperatura do chip = %.2f\r\n", temperatura);
        if(temperatura < 45){
            printf("Faixa Segura de Operacao\r\n");
        }
        else if(temperatura <= 55){
            printf("Faixa de Atencao\r\n");
        }
        else{
            printf("Faixa de Superaquecimento\r\n");
        }

        Delay_ms(100);
    }
}

void questao9_prova2(void){

    USART1_Init();

    ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
    ADC_SingleChannel(ADC1, PIN_0);
    GPIO_Resistor_Enable(GPIOA, PIN_0, PULL_DOWN);

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 41999;
    TIM3->ARR = 0xFFFF;
    TIM3->CNT = 0;
    TIM3->EGR = TIM_EGR_UG;
    TIM3->SR = 0;
    TIM3->CR1 |= TIM_CR1_CEN;

    uint8_t estado = 0;
    uint32_t largura_ms = 0;
    uint8_t contador_subida = 0;
    uint8_t contador_descida = 0;

    while(1){
        float soma = 0.0f;
        for(int i = 0; i < 50; i++){
            soma += ADC_rotiner_escalonador(ADC1, PIN_0, 0.0f, 3.3f);
        }
        float leitura = soma / 50.0f;

        if(estado == 0){
            if(leitura > 2.0f){
                contador_subida++;
                if(contador_subida >= 5){
                    TIM3->CNT = 0;
                    printf("Pulso iniciado...\r\n");
                    estado = 1;
                    contador_subida = 0;
                }
            } else {
                contador_subida = 0;
            }
        }

        else if(estado == 1){
            largura_ms = TIM3->CNT / 2;

            if(leitura <= 2.0f){
                contador_descida++;
                if(contador_descida >= 3){
                    printf("Fim do pulso! Duracao total: %lu ms\r\n", largura_ms);
                    TIM3->CNT = 0;
                    estado = 2;
                    contador_descida = 0;
                }
            } else {
                contador_descida = 0;
            }

            if(largura_ms >= 5000){
                printf("Pulso muito largo! (> 5000 ms)\r\n");
                TIM3->CNT = 0;
                estado = 2;
                contador_descida = 0;
            }
            else if (estado == 1) {
                printf("Tempo atual: %lu ms\r\n", largura_ms);
            }
        }

        else if(estado == 2){
            if(leitura > 2.0f){
                TIM3->CNT = 0;
            }
            else if(TIM3->CNT >= 20){
                estado = 0;
            }
        }
    }
}

void TIM6Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC = 0;
    TIM6->ARR = 1905 - 1;
    TIM6->CNT = 0;
    TIM6->CR1 |= TIM_CR1_CEN;
}

void questao6_reproduzir_voz(void) {
    DAC_Init1(DAC_CHANNEL2);
    TIM6Init();


    while(1) {
        for (uint32_t i = 0; i < TAMANHO_AUDIO; i++) {
            while (!(TIM6->SR & TIM_SR_UIF));
            TIM6->SR &= ~TIM_SR_UIF;
            uint32_t valor_12bits = ((uint32_t)audio_voz[i]) << 4;
            DAC_SetValue(DAC_CHANNEL2, valor_12bits, DAC_RES_12BITS);
        }
        Delay_ms(2000);
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
  USART1_Init();


  /* Chame aqui a função referente a rotina */
  questao6_reproduzir_voz();

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

void SysTick_Handler(){
	GPIO_Toggle_Pin(GPIOA, PIN_7);
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
