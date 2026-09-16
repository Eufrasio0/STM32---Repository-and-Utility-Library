#ifndef ATVVAVALIATIVAONE_H_
#define ATVVAVALIATIVAONE_H_

#include "stm32f4xx.h"
#include "Utility.h"

void questao1(){

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);

	while(1){
		GPIO_Toggle_Pin(GPIOA, PIN_6);
		Delay_ms(250);
		GPIO_Toggle_Pin(GPIOA, PIN_6);
		Delay_ms(250);
	}

}

void questao3(){

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);

	while(1){
		for(int i = 0; i < 4; i++){
			GPIO_Toggle_Pin(GPIOA, PIN_6);
			Delay_ms(50);
			GPIO_Toggle_Pin(GPIOA, PIN_6);
			Delay_ms(50);
		}
		Delay_ms(2000);
	}
}

void questao5() {

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT);

	while(1){
		for (int i = 0; i < 8; i++) {
			GPIOA->BSRR = (7 << 16);
			GPIOA->BSRR = i;
			Delay_ms(500);

		}
	}
}

void questao7(){

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_4, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_5, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);

	while(1){
		for(int i = 0; i < 256;i++){
			GPIOA->BSRR = (255 << 16);
			GPIOA->BSRR = i;
			Delay_ms(500);
		}
	}
}

void questao9(){

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_4, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_5, OUTPUT);

	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_8, OUTPUT);

	int sinal = 0;

	while(1){
		if(sinal == 0){
			GPIO_Toggle_Pin(GPIOA, PIN_3);
			Delay_ms(3000);
			GPIO_Toggle_Pin(GPIOA, PIN_3);
			GPIO_Toggle_Pin(GPIOA, PIN_4);
			Delay_ms(2000);
			GPIO_Toggle_Pin(GPIOA, PIN_4);
			GPIO_Toggle_Pin(GPIOA, PIN_5);
			Delay_ms(5000);
			GPIO_Toggle_Pin(GPIOA, PIN_5);
			Delay_ms(1000);
			sinal++;
		}else{
			GPIO_Toggle_Pin(GPIOA, PIN_6);
			Delay_ms(3000);
			GPIO_Toggle_Pin(GPIOA, PIN_6);
			GPIO_Toggle_Pin(GPIOA, PIN_7);
			Delay_ms(2000);
			GPIO_Toggle_Pin(GPIOA, PIN_7);
			GPIO_Toggle_Pin(GPIOA, PIN_8);
			Delay_ms(5000);
			GPIO_Toggle_Pin(GPIOA, PIN_8);
			Delay_ms(1000);
			sinal--;
		}
	}
}

void questao11(){
	GPIO_Clock_Enable(GPIOA);
//os pinos estao na ordem alfabetica
	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_4, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_5, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);

	int mascaras[16] = {
			0b1011111, //0
			0b1010000, //1
			0b0111011, //2
			0b0101111, //3
			0b1100110, //4
			0b1101101, //5
			0b1111101, //6
			0b0000111, //7
			0b1111111, //8
			0b1101111, //9
			0b1110111, //A
			0b1111111, //B
			0b1011001, //C
			0b1011111, //D
			0b1111001, //E
			0b1110001 //F
	};
	while(1){
		for(int i = 0; i < 16;i++){
			GPIOA->BSRR = (0b1111111 << 16);
			GPIOA->BSRR = mascaras[i];
			Delay_ms(1000);
		}
	}
}

void comp(GPIO_TypeDef* GPIOx, uint8_t PINO, int angulo){

	  GPIO_Clock_Enable(GPIOx);
	    GPIO_Pin_Mode(GPIOx, PINO, OUTPUT);

	    int pulso = 500 + (angulo * 2000) / 180;
	    for(int i = 0; i < 25; i++)
	    {
	        GPIO_Write_Pin(GPIOx, PINO, HIGH);
	        Delay_us(pulso);

	        GPIO_Write_Pin(GPIOx, PINO, LOW);
	        Delay_us(20000 - pulso);
	    }
}

void questao13(){
	comp(GPIOA, PIN_0,0);
    Delay_ms(1000);

    comp(GPIOA, PIN_0,180);
    Delay_ms(1000);

    comp(GPIOA, PIN_0,0);
    Delay_ms(1000);



}

void questao15()
{
    GPIO_Clock_Enable(GPIOA);

    GPIO_Pin_Mode(GPIOA, PIN_0, INPUT);
    GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);

    GPIO_Resistor_Enable(GPIOA, PIN_0, PULL_UP);

    int estado = LOW;

    while(1)
    {
        if ((GPIOA->IDR & (1 << 0)) == 0)
        {
            estado = !estado;
            GPIO_Write_Pin(GPIOA, PIN_1, estado);

            while ((GPIOA->IDR & (1 << 0)) == 0)
            {
            }
            Delay_ms(30);
        }
    }
}

#endif /*ATVVAVALIATIVAONE_H_*/
