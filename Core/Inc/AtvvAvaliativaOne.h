#ifndef BIBUSTM32
#define BIBUSTM32_H_

#include "stm32f4xx.h"
#include "Utility.h"

void questao1(){
	while(1){
		GPIO_Toggle_Pin(GPIOA, PIN_6);
		Delay_ms(250);
		GPIO_Toggle_Pin(GPIOA, PIN_6);
		Delay_ms(250);
	}

}

void questao3(){
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

void questao5(){
	for(int i = 0; i < 3;i++){
		GPIOA->BSRR = i;
	}
}

void questao7(){
	for(int i = 0; i < 8;i++){
		GPIOA->BSRR = i;
	}
}

void questao9(){

	int sinal = 0;

	while(1){
		if(sinal == 0){
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


}

#endif
