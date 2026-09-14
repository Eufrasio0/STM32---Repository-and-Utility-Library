#ifndef BIBUSTM32
#define BIBUSTM32_H_

#include "stm32f4xx.h"


//Números dos pinos GPIO
enum{
	PIN_0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	PIN_10,
	PIN_11,
	PIN_12,
	PIN_13,
	PIN_14,
	PIN_15
};

//Modos de operação de um pino GPIO
enum{
	INPUT,		//modo de entrada digital
	OUTPUT,		//modo de saída digital
	ALTERNATE,	//modo de função alternativa
	ANALOG		//modo analógico
};

//Tipos de saída de um pino GPIO
enum{
	PUSH_PULL,	//saída push-pull
	OPEN_DRAIN	//saída open-drain
};

//Níveis lógicos de saída de um pino
enum{
	LOW,	//nível lógico baixo
	HIGH	//nível lógico alto
};

//Modos de operação dos resistores de pull-up e pull-down
enum{
	PULL_UP = 0b01,		//resistor de pull-up
	PULL_DOWN = 0b10	//resistor de pull-down
};


//Escopo das funções

void GPIO_Output_Type(GPIO_TypeDef* GPIOx, uint8_t PINO, uint8_t MODE);
void GPIO_Clock_Enable(GPIO_TypeDef* GPIOx);


void GPIO_Clock_Enable(GPIO_TypeDef* GPIOx)
{
	RCC->AHB1ENR |= (1 << ((uint32_t)GPIOx - (GPIOA_BASE)) / ((GPIOB_BASE) - (GPIOA_BASE)));
}

void GPIO_Output_Type(GPIO_TypeDef* GPIOx, uint8_t PINO, uint8_t MODE){
	GPIOx->OTYPER &= ~(1 << PINO);
	GPIOx->OTYPER |= (MODE << PINO);
}

void GPIO_Output_Type(GPIO_TypeDef* GPIOx, uint8_t PINO, uint8_t MODE){
	GPIOx->OTYPER &= ~(1 << PINO);
	GPIOx->OTYPER |= (MODE << PINO);
}



#endif 
