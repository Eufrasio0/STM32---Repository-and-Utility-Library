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
		Delay_ms(400);
	}
}

void questao5() {

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT);


    for (int i = 0; i < 8; i++) {
        GPIOA->BSRR = (7 << 16);
        GPIOA->BSRR = i;
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


	for(int i = 0; i < 256;i++){
        GPIOA->BSRR = (255 << 16);
		GPIOA->BSRR = i;
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

	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_4, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_5, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);

	const int mascaras[16] = {
			0b1011111,
			0b1010000,
			0b0111011,
			0b0101111,
			0b1100110,
			0b1101101,
			0b1111101,
			0b0000111,
			0b1111111,
			0b1101111,
			0b1110111,
			0b1111111,
			0b1011001,
			0b1011111,
			0b1111001,
			0b1110001
	};

	for(int i = 0; i < 16;i++){
        GPIOA->BSRR = (0b1111111 << 16);
        GPIOA->BSRR = mascara[i];
	}
}

#endif /*ATVVAVALIATIVAONE_H_*/
