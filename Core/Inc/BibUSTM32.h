#ifndef BIBUSTM32
#define BIBUSTM32_H_

#include "stm32f4xx.h"
#include "Utility.h"

static uint32_t elapsed_start_cycles = 0;

enum{
	PINO_0,
	PINO_1,
	PINO_2,
	PINO_3,
	PINO_4,
	PINO_5,
	PINO_6,
	PINO_7,
	PINO_8,
	PINO_9,
	PINO_10,
	PINO_11,
	PINO_12,
	PINO_13,
	PINO_14,
	PINO_15
};

enum{
	IN,
	OUT,
	ALTER,
	ANALOGIC
};

enum{
	BAIXO,
	ALTO	//nível lógico alto
};

enum{
	PULLUP = 0b01,
	PULLDOWN = 0b10
};

enum{
	FALL_EDGE = 1,
	RIS_EDGE = 2
};

void Configure_Clock_Init(void){
	#define PLL_M	4
	#define PLL_N	168
	#define PLL_P	2
	#define PLL_Q	7

	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;	//habilita prefetch, caches e a latência da flash
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;	//habilita o clock da interface de energia
	PWR->CR |= PWR_CR_VOS;				//regulador de tensão no modo 2
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	RCC->CIR = 0;				//desabilita todas as interrupções de RCC
	RCC->CR |= RCC_CR_HSION;	//liga o oscilador HSI
	RCC->CFGR = 0;				//reseta o registrador CFGR
	RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON | RCC_CR_HSEBYP);
	RCC->PLLCFGR = 0x24003010;	//reseta o registrador PLLCFGR

	RCC->CR |= RCC_CR_HSEON;				//habilita HSE
	while(!((RCC->CR) & RCC_CR_HSERDY));	//espera HSE ficar pronto
    RCC->CFGR |= 0x9400;	//HCLK = SYSCLK/1, PCLK2 = HCLK/2, PCLK1 = HCLK/4
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) | (0x400000) | (PLL_Q << 24);

    RCC->CR |= RCC_CR_PLLON;			//habilita o PLL
    while(!(RCC->CR & RCC_CR_PLLRDY));	//espera o PLL ficar pronto verificando a flag PLLRDY

    RCC->CFGR |= 0x2;					//seleciona o PLL como fonte de SYSCLK
    while((RCC->CFGR & 0xC) != 0x8);	//espera o PLL ser a fonte de SYSCLK
}

void timer2_Setup(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	//liga o clock do Timer2
	TIM2->PSC = 83;						//prescaler para incrementos a cada 1uS
	TIM2->EGR = TIM_EGR_UG;				//update event para escrever o valor do prescaler
	TIM2->CR1 |= TIM_CR1_CEN;			//habilita o timer
}

void delay_us(uint32_t delay){
	TIM2->CNT = 0;
	while(TIM2->CNT < delay);
}

void delay_ms(uint32_t delay){
	uint32_t max = 1000*delay;
	TIM2->CNT = 0;
	while(TIM2->CNT < max);
}

void Elapsed_Start(void){
    elapsed_start_cycles = DWT->CYCCNT;
}

uint32_t Elapsed_us(void){
    uint32_t cycles =
        (uint32_t)(DWT->CYCCNT - elapsed_start_cycles);

    return cycles / (SystemCoreClock / 1000000U);
}

uint32_t Elapsed_ms(void)
{
    return Elapsed_us() / 1000U;
}


void GPIO_PinConfig(){

}


void escalaMusical(GPIO_TypeDef* GPIOx, uint8_t PINO){
	  while(1){
		  frequencia(GPIOx, PINO, 261);
		  Delay_ms(500);
		  frequencia(GPIOx, PINO,293);
		  Delay_ms(500);
		  frequencia(GPIOx, PINO,329);
		  Delay_ms(500);
		  frequencia(GPIOx, PINO,349);
		  Delay_ms(500);
		  frequencia(GPIOx, PINO,392);
		  Delay_ms(500);
		  frequencia(GPIOx, PINO,440);
		  Delay_ms(500);
		  frequencia(GPIOx, PINO,493);
		  Delay_ms(500);
		  frequencia(GPIOx, PINO,523);
		  Delay_ms(500);
	  }
}


void servoSetGrau(GPIO_TypeDef* GPIOx, uint8_t PINO, int angulo)
{
    GPIO_Clock_Enable(GPIOx);
    GPIO_Pin_Mode(GPIOx, PINO, OUTPUT);

    int pulso = 1000 + (angulo * 1000) / 180;

    for(int i = 0; i < 25; i++)
    {
        GPIO_Write_Pin(GPIOx, PINO, HIGH);
        Delay_us(pulso);

        GPIO_Write_Pin(GPIOx, PINO, LOW);
        Delay_us(20000 - pulso);
    }
}

void interrupcao(){
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
}

int getdistancia(int Trigger, int Echo)
{
    uint32_t retorno = 0;

    GPIO_Write_Pin(GPIOA, Trigger, LOW);
    Delay_ms(1);

    GPIO_Write_Pin(GPIOA, Trigger, HIGH);
    Delay_us(10);
    GPIO_Write_Pin(GPIOA, Trigger, LOW);

    while (!GPIO_Read_Pin(GPIOA, Echo));

    while (GPIO_Read_Pin(GPIOA, Echo) && retorno < 23200)
    {
        retorno++;
        Delay_us(1);
    }

    return retorno / 58;
}


void USART_Init(void){

	//No serial monitor coloca o bound rate para 115200
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    GPIOA->MODER &= ~((0b11U << 4) | (0b11U << 6));
    GPIOA->MODER |=  ((0b10U << 4) | (0b10U << 6));

    // 4. Seleciona AF7 para PA2 e PA3
    GPIOA->AFR[0] &= ~((0b1111U << 8) | (0b1111U << 12));
    GPIOA->AFR[0] |=  ((0b0111U << 8) | (0b0111U << 12));

    // PCLK1 = 42 MHz; oversampling por 16
    USART2->BRR = (42000000U + 57600U) / 115200U;
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 |= USART_CR1_UE;
}

void I2C_Init(GPIO_TypeDef* GPIOx, int SDA, int SCL){
	if(GPIOx == GPIOA){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}
	else if(GPIOx == GPIOB){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	}else if(GPIOx == GPIOC){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	}else if(GPIOx == GPIOD){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	}else if(GPIOx == GPIOE){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	}
	GPIOx->MODER |= (11 << SDA);
	GPIOx->MODER |= (11 << SCL);

}





#endif 
