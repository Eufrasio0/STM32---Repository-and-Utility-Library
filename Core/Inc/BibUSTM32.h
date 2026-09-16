#ifndef BIBUSTM32
#define BIBUSTM32_H_

#include "stm32f4xx.h"
#include "Utility.h"


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


void servo(GPIO_TypeDef* GPIOx, uint8_t PINO, int angulo)
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

void PWM_LED(int tempo)
{
    int marc = 0;
    GPIO_Clock_Enable(GPIOA);
    GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);


    while(1)
    {
        GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
        Delay_us(tempo);

        GPIO_Write_Pin(GPIOA, PIN_6, LOW);
        Delay_us(10000 - tempo);

        if(marc == 0)
            tempo -= 100;

        if(marc == 1)
            tempo += 100;

        if(tempo <= 0){
            marc = 1;
        }

        if(tempo >= 10000){
            marc = 0;
        }
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

	EXTI_Config(EXTI3, GPIOE, FALLING_EDGE);
	EXTI_Config(EXTI4, GPIOE, FALLING_EDGE);

	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);


	NVIC_SetPriority(EXTI3_IRQn, 0);
	NVIC_SetPriority(EXTI4_IRQn, -1);
}

void EXTI3_IRQHandler(){
	//Digite aqui a rotina da interrupçao
	EXTI_Clear_Pending(EXTI3);
}

void EXTI4_IRQHandler(){
	//Digite aqui a rotina da interrupçao
	EXTI_Clear_Pending(EXTI4);

}




#endif 
