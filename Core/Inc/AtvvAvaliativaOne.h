#ifndef ATVVAVALIATIVAONE_H_
#define ATVVAVALIATIVAONE_H_

#include "stm32f4xx.h"
#include "Utility.h"
#include "LCD_Blio.h"
#include "stdio.h"

//Declaracao das flags
volatile uint8_t flag = 0;
volatile uint8_t flag_seg = 0;

volatile int pedestre= 0;
volatile int cont=0;

volatile int flag_K0;
volatile int flag_K1;
volatile int sequencia_valida;


char Keypad_Read(void)
{
    // Linha 0
    GPIO_Write_Pin(GPIOA, PIN_0, LOW);
    GPIO_Write_Pin(GPIOA, PIN_1, HIGH);
    GPIO_Write_Pin(GPIOA, PIN_2, HIGH);
    GPIO_Write_Pin(GPIOA, PIN_3, HIGH);

    Delay_us(10);

    if (GPIO_Read_Pin(GPIOA, PIN_4) == LOW) return '1';
    if (GPIO_Read_Pin(GPIOA, PIN_5) == LOW) return '2';
    if (GPIO_Read_Pin(GPIOA, PIN_6) == LOW) return '3';
    if (GPIO_Read_Pin(GPIOA, PIN_7) == LOW) return 'A';


    // Linha 1
    GPIO_Write_Pin(GPIOA, PIN_0, HIGH);
    GPIO_Write_Pin(GPIOA, PIN_1, LOW);

    Delay_us(10);

    if (GPIO_Read_Pin(GPIOA, PIN_4) == LOW) return '4';
    if (GPIO_Read_Pin(GPIOA, PIN_5) == LOW) return '5';
    if (GPIO_Read_Pin(GPIOA, PIN_6) == LOW) return '6';
    if (GPIO_Read_Pin(GPIOA, PIN_7) == LOW) return 'B';


    // Linha 2
    GPIO_Write_Pin(GPIOA, PIN_1, HIGH);
    GPIO_Write_Pin(GPIOA, PIN_2, LOW);

    Delay_us(10);

    if (GPIO_Read_Pin(GPIOA, PIN_4) == LOW) return '7';
    if (GPIO_Read_Pin(GPIOA, PIN_5) == LOW) return '8';
    if (GPIO_Read_Pin(GPIOA, PIN_6) == LOW) return '9';
    if (GPIO_Read_Pin(GPIOA, PIN_7) == LOW) return 'C';


    // Linha 3
    GPIO_Write_Pin(GPIOA, PIN_2, HIGH);
    GPIO_Write_Pin(GPIOA, PIN_3, LOW);

    Delay_us(10);

    if (GPIO_Read_Pin(GPIOA, PIN_4) == LOW) return '*';
    if (GPIO_Read_Pin(GPIOA, PIN_5) == LOW) return '0';
    if (GPIO_Read_Pin(GPIOA, PIN_6) == LOW) return '#';
    if (GPIO_Read_Pin(GPIOA, PIN_7) == LOW) return 'D';

    return 0;
}

int rotina_de_espera()
{
	TIM2_Setup();
    TIM2->CNT = 0;

    while (TIM2->CNT < 1000000)
    {
        if (GPIO_Read_Pin(GPIOE, PIN_3) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int distancia(int Trigger, int Echo)
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

void frequencia(GPIO_TypeDef* GPIOx, uint8_t PINO ,int freq)
{
    GPIO_Clock_Enable(GPIOx);
    GPIO_Pin_Mode(GPIOx, PINO, OUTPUT);
    int periodo = 1000000 / freq;
    int meio_periodo = periodo / 2;
    int ciclos = freq / 2;

    for(int i = 0; i < ciclos; i++)
    {
        GPIO_Write_Pin(GPIOx, PINO, HIGH);
        Delay_us(meio_periodo);

        GPIO_Write_Pin(GPIOx, PINO, LOW);
        Delay_us(meio_periodo);
    }
}

void beep(GPIO_TypeDef* GPIOx, uint8_t PINO ,int freq, int quant){
	for(int i = 0; i < quant;i++){
		frequencia(GPIOx, PINO, freq);
		Delay_ms(500);
		frequencia(GPIOx, PINO, (freq + 100));
	}
}

void piscar_led(int PINO){
		GPIO_Toggle_Pin(GPIOA, PINO);
		Delay_ms(50);
}

void semaforo_pedestre(int PINO1, int PINO2)
{
    GPIO_Write_Pin(GPIOA, PINO1, HIGH);
    GPIO_Write_Pin(GPIOA, PIN_2, HIGH);

    Delay_ms(3000);

    for(int i = 0; i < 10; i++)
    {
        GPIO_Write_Pin(GPIOA, PINO2, HIGH);
        Delay_ms(200);

        GPIO_Write_Pin(GPIOA, PINO2, LOW);
        Delay_ms(200);
    }
    GPIO_Write_Pin(GPIOA, PIN_2, LOW);

    GPIO_Write_Pin(GPIOA, PINO1, LOW);
    GPIO_Write_Pin(GPIOA, PINO2, LOW);

    pedestre = 0;
}

void semaforo(int PINO){
	GPIO_Toggle_Pin(GPIOA, PINO);
	if(PINO == PIN_0 || PINO == PIN_1){
		GPIO_Write_Pin(GPIOA, PIN_4, HIGH);
	}

	for(int i = 0; i < 100; i++){
		if(GPIO_Read_Pin(GPIOA, PIN_0) == 1 && pedestre == 1) break;
		Delay_ms(30);
	}
	GPIO_Write_Pin(GPIOA, PIN_4, LOW);

	GPIO_Toggle_Pin(GPIOA, PINO);

}




void EXTI0_IRQHandler(){
	//Digite aqui a rotina da interrupçaoaaaaaaa
	while(flag == 1){
		piscar_led(PIN_3);
	}
	flag = 1;
}

void EXTI1_IRQHandler(){
	//Digite aqui a rotina da interrupçao
	while(flag == 2){
		piscar_led(PIN_4);
	}
	flag = 2;
}

void EXTI2_IRQHandler(){

	GPIO_Write_Pin(GPIOA, PIN_3, LOW);
	GPIO_Write_Pin(GPIOA, PIN_4, LOW);
	if(flag == 2){
		EXTI_Clear_Pending(EXTI0);
		EXTI_Clear_Pending(EXTI1);
		flag = 0;
	}
	if(flag == 1){
		EXTI_Clear_Pending(EXTI1);
		EXTI_Clear_Pending(EXTI0);
		flag = 0;
	}

	EXTI_Clear_Pending(EXTI2);
}

void EXTI3_IRQHandler(){
	/*pedestre = 1;
	EXTI_Clear_Pending(EXTI3);
	questao do semaforo
	*/

	if(flag == 1){
		GPIO_Write_Pin(GPIOE, PIN_2, HIGH);
	}


}

void EXTI4_IRQHandler(){
	/*
	Delay_ms(10);
	GPIO_Toggle_Pin(GPIOA, PIN_1);
	EXTI_Clear_Pending(EXTI4);

	questao do semaforo*/
	TIM2_Setup();
	    TIM2->CNT = 0;

	    while (TIM2->CNT < 1000000){
	    	flag = 1;
	    }
	    flag = 0;

}

void servomotor(GPIO_TypeDef* GPIOx, uint8_t PINO, int angulo){

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

void questao1(){

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);

	while(1){
		GPIO_Toggle_Pin(GPIOA, PIN_0);
		Delay_ms(250);
		GPIO_Toggle_Pin(GPIOA, PIN_0);
		Delay_ms(250);
	}
}

void questao3(){

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);

	while(1){
		for(int i = 0; i < 4; i++){
			GPIO_Toggle_Pin(GPIOA, PIN_0);
			Delay_ms(50);
			GPIO_Toggle_Pin(GPIOA, PIN_0);
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

void questao8(){
	Utility_Init(); //Inicia o uso da biblioteca

	GPIO_Clock_Enable(GPIOD); //Ativa a porta D

	GPIO_Pin_Mode(GPIOD, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_2, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_3, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_4, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_5, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_7, OUTPUT);

	GPIO_Write_Pin(GPIOD, PIN_0, LOW);
	GPIO_Write_Pin(GPIOD, PIN_1, LOW);
	GPIO_Write_Pin(GPIOD, PIN_2, LOW);
	GPIO_Write_Pin(GPIOD, PIN_3, LOW);
	GPIO_Write_Pin(GPIOD, PIN_4, LOW);
	GPIO_Write_Pin(GPIOD, PIN_5, LOW);
	GPIO_Write_Pin(GPIOD, PIN_6, LOW);
	GPIO_Write_Pin(GPIOD, PIN_7, LOW);


	GPIO_Toggle_Pin(GPIOD, PIN_0);
	Delay_ms(400);
	GPIO_Toggle_Pin(GPIOD, PIN_0);

	while(1){
		for (int i=1; i<8;i++){
			GPIO_Toggle_Pin(GPIOD, i);
			Delay_ms(400);
			GPIO_Toggle_Pin(GPIOD, i);
		}

		for (int j=6; j>=0;j--){
			GPIO_Toggle_Pin(GPIOD, j);
			Delay_ms(400);
			GPIO_Toggle_Pin(GPIOD, j);
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


	while(1){
		GPIO_Toggle_Pin(GPIOA, PIN_3);
		GPIO_Toggle_Pin(GPIOA, PIN_8);
		Delay_ms(2000);
		GPIO_Toggle_Pin(GPIOA, PIN_3);
		GPIO_Toggle_Pin(GPIOA, PIN_8);
		GPIO_Toggle_Pin(GPIOA, PIN_4);
		GPIO_Toggle_Pin(GPIOA, PIN_7);
		Delay_ms(2000);
		GPIO_Toggle_Pin(GPIOA, PIN_4);
		GPIO_Toggle_Pin(GPIOA, PIN_7);
		GPIO_Toggle_Pin(GPIOA, PIN_5);
		GPIO_Toggle_Pin(GPIOA, PIN_6);
		Delay_ms(1000);
		GPIO_Toggle_Pin(GPIOA, PIN_5);
		GPIO_Toggle_Pin(GPIOA, PIN_6);

	}
}

void questao10(){
	Utility_Init(); //Inicia o uso da biblioteca

	GPIO_Clock_Enable(GPIOA); //Ativa a porta D
	GPIO_Clock_Enable(GPIOE); //Ativa a porta D


	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_4, OUTPUT);

	GPIO_Pin_Mode(GPIOE, PIN_3, INPUT); //configura o pino 3E como entrada
	GPIO_Resistor_Enable(GPIOE, PIN_3, PULL_UP);//ativa o registrador de pull-up do pino 3E
	EXTI_Config(EXTI3, GPIOE, FALLING_EDGE);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_SetPriority(EXTI3_IRQn, 0);

    while (1)
    {
        while (pedestre == 0)
        {
            semaforo(PIN_0);
            semaforo(PIN_1);
            semaforo(PIN_2);
        }

        semaforo_pedestre(PIN_3, PIN_4);
    }


}


void questao11(){
	GPIO_Clock_Enable(GPIOD);
//os pinos estao na ordem alfabetica
	GPIO_Pin_Mode(GPIOD, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_2, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_3, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_4, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_5, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_6, OUTPUT);

	int mascaras[16] = {
		    0b1000000, // 0
		    0b1111001, // 1
		    0b0100100, // 2
		    0b0110000, // 3
		    0b0011001, // 4
		    0b0010010, // 5
		    0b0000010, // 6
		    0b1111000, // 7
		    0b0000000, // 8
		    0b0010000, // 9
		    0b0001000, // A
		    0b0000000, // B
		    0b1000110, // C
		    0b1000000, // D
		    0b0000110, // E
		    0b0001110  // F
		};


	while(1){
		for(int i = 0; i < 16;i++){
			GPIOD->BSRR = (0b1111111 << 16);
			GPIOD->BSRR = mascaras[i];
			Delay_ms(1000);
		}
		for(int i = 16; i != 0;i--){
			GPIOD->BSRR = (0b1111111 << 16);
			GPIOD->BSRR = mascaras[i];
			Delay_ms(1000);
		}

	}
}

void questao12(){

    char *texto[] = {
        "0", "1", "2", "3", "4", "5",
        "6", "7", "8", "9", "10"
    };

    LCD_Init(4, 20);
    LCD_Clear();

    LCD_Write_String(1, 1, "Eufrasio");
    LCD_Write_String(2, 1, "Arruda");
    LCD_Write_String(3, 1, "Neto");

    while(1){
        for(int i = 10; i != 0; i++){
            LCD_Write_String(4, 1, texto[i]);
            Delay_ms(1000);

            LCD_Write_String(4, 1, "                    ");
        }
    }
}




void questao13(){
	Utility_Init(); //Inicia o uso da biblioteca
	GPIO_Clock_Enable(GPIOA); //Ativa a porta A

	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);

	GPIO_Write_Pin(GPIOA, PIN_0, LOW);

	while(1){
		for (int i=500; i<=2500;i=i+50){
			GPIO_Toggle_Pin(GPIOA, PIN_0);
			Delay_us(i);
			GPIO_Toggle_Pin(GPIOA, PIN_0);
			Delay_ms(20);
		}
		for (int i=2500; i>=500;i=i-50){
			GPIO_Toggle_Pin(GPIOA, PIN_0);
			Delay_us(i);
			GPIO_Toggle_Pin(GPIOA, PIN_0);
			Delay_ms(20);
		}
	}

}

void questao15()
{
    GPIO_Clock_Enable(GPIOA);
    GPIO_Clock_Enable(GPIOE);


    GPIO_Pin_Mode(GPIOE, PIN_4, INPUT);
    GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);

    GPIO_Resistor_Enable(GPIOE, PIN_4, PULL_UP);

	EXTI_Config(EXTI4, GPIOE, FALLING_EDGE);
	NVIC_EnableIRQ(EXTI4_IRQn);


}

void questao16(){
    GPIO_Clock_Enable(GPIOE);

    GPIO_Pin_Mode(GPIOE, PIN_2, OUTPUT);
    GPIO_Pin_Mode(GPIOE, PIN_3, INPUT);   // K1
    GPIO_Pin_Mode(GPIOE, PIN_4, INPUT);   // K0

    GPIO_Resistor_Enable(GPIOE, PIN_3, PULL_UP);
    GPIO_Resistor_Enable(GPIOE, PIN_4, PULL_UP);

    TIM2_Setup();
    GPIO_Write_Pin(GPIOE, PIN_2, LOW);

    while (1)
    {
        flag_K0 = 0;
        flag_K1 = 0;
        sequencia_valida = 0;

        if (GPIO_Read_Pin(GPIOE, PIN_3) == 0)
        {
            continue;
        }
        while (GPIO_Read_Pin(GPIOE, PIN_4) != 0)
        {
            if (GPIO_Read_Pin(GPIOE, PIN_3) == 0)
            {
                flag_K1 = 1;
                break;
            }
        }
        if (flag_K1 == 1)
        {
            continue;
        }
        flag_K0 = 1;
        TIM2->CNT = 0;
        while (TIM2->CNT < 1000000)
        {
            if (GPIO_Read_Pin(GPIOE, PIN_3) == 0)
            {
                flag_K1 = 1;
                sequencia_valida = 1;
                break;
            }
        }
        if ((flag_K0 == 1) && (flag_K1 == 1) && (sequencia_valida == 1))
        {
            GPIO_Write_Pin(GPIOE, PIN_2, HIGH);
        }
        while ((GPIO_Read_Pin(GPIOE, PIN_3) == 0) ||(GPIO_Read_Pin(GPIOE, PIN_4) == 0)){

        }
        GPIO_Write_Pin(GPIOE, PIN_2, LOW);

    }

}



void questao17(){

	/*Pedro, c vai montar seguindo os esquemas descritos abaixo*/

    GPIO_Clock_Enable(GPIOA);
//botoes
    GPIO_Pin_Mode(GPIOA, PIN_0, INPUT);
    GPIO_Pin_Mode(GPIOA, PIN_1, INPUT);
    GPIO_Pin_Mode(GPIOA, PIN_2, INPUT);

    GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);
    GPIO_Pin_Mode(GPIOA, PIN_4, OUTPUT);


    GPIO_Resistor_Enable(GPIOA, PIN_0, PULL_UP);
    GPIO_Resistor_Enable(GPIOA, PIN_1, PULL_UP);
    GPIO_Resistor_Enable(GPIOA, PIN_2, PULL_UP);

	EXTI_Config(EXTI0, GPIOA, FALLING_EDGE);
	EXTI_Config(EXTI1, GPIOA, FALLING_EDGE);
	EXTI_Config(EXTI2, GPIOA, FALLING_EDGE);

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);

	NVIC_SetPriority(EXTI0_IRQn, 1);
	NVIC_SetPriority(EXTI1_IRQn, 1);
	NVIC_SetPriority(EXTI2_IRQn, 0);
}

/*
void questao19()
{
    GPIO_Clock_Enable(GPIOA);
    GPIO_Clock_Enable(GPIOD);

    GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
    GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
    GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT);
    GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);

    GPIO_Output_Type(GPIOA, PIN_0, OPEN_DRAIN);
    GPIO_Output_Type(GPIOA, PIN_1, OPEN_DRAIN);
    GPIO_Output_Type(GPIOA, PIN_2, OPEN_DRAIN);
    GPIO_Output_Type(GPIOA, PIN_3, OPEN_DRAIN);

    GPIO_Resistor_Enable(GPIOA, PIN_0, PULL_UP);
    GPIO_Resistor_Enable(GPIOA, PIN_1, PULL_UP);
    GPIO_Resistor_Enable(GPIOA, PIN_2, PULL_UP);
    GPIO_Resistor_Enable(GPIOA, PIN_3, PULL_UP);

    GPIO_Resistor_Enable(GPIOA, PIN_4, PULL_UP);
    GPIO_Resistor_Enable(GPIOA, PIN_5, PULL_UP);
    GPIO_Resistor_Enable(GPIOA, PIN_6, PULL_UP);
    GPIO_Resistor_Enable(GPIOA, PIN_7, PULL_UP);

    GPIO_Pin_Mode(GPIOA, PIN_4, INPUT);
    GPIO_Pin_Mode(GPIOA, PIN_5, INPUT);
    GPIO_Pin_Mode(GPIOA, PIN_6, INPUT);
    GPIO_Pin_Mode(GPIOA, PIN_7, INPUT);

    //display
    GPIO_Pin_Mode(GPIOD, PIN_0, OUTPUT);
    GPIO_Pin_Mode(GPIOD, PIN_1, OUTPUT);
    GPIO_Pin_Mode(GPIOD, PIN_2, OUTPUT);
    GPIO_Pin_Mode(GPIOD, PIN_3, OUTPUT);
    GPIO_Pin_Mode(GPIOD, PIN_4, OUTPUT);
    GPIO_Pin_Mode(GPIOD, PIN_5, OUTPUT);
    GPIO_Pin_Mode(GPIOD, PIN_6, OUTPUT);

    int mascara[] = {

        0b1000000, // 0
        0b1111001, // 1
        0b0100100, // 2
        0b0110000, // 3
        0b0011001, // 4
        0b0010010, // 5
        0b0000010, // 6
        0b1111000, // 7
        0b0000000, // 8
        0b0010000, // 9
        0b0001000, // A
        0b0000000, // B
        0b1000110, // C
        0b1000000, // D
        0b0000110, // E
        0b0001110  // F
    };

    while (1)
    {
        char tecla;

        tecla = Keypad_Read();

			if (tecla == '1'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[1];
			}
			else if(tecla == '0'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[0];
			}
			else if(tecla == '2'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[2];
			}
			else if(tecla == '3'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[3];
			}
			else if(tecla == '4'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[4];
			}
			else if(tecla == '5'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[5];
			}
			else if(tecla == '6'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[6];
			}
			else if(tecla == '7'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[7];
			}
			else if(tecla == '8'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[8];
			}
			else if(tecla == '9'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[9];
			}
			else if(tecla == "A"){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[10];
			}
			else if(tecla == 'B'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[11];
			}
			else if(tecla == 'C'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[12];
			}
			else if(tecla == 'D'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[13];
			}
			else if(tecla == '*'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[14];
			}
			else if(tecla == '#'){
				GPIOA->BSRR = (0b1111111 << 16);
				GPIOA->BSRR = mascara[15];
			}

        }
}

void questao20(){
    GPIO_Clock_Enable(GPIOA);
    GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT); // trigger
    GPIO_Pin_Mode(GPIOA, PIN_1, INPUT); // echo
    GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT); // buzzer

    while(1){
    	int dist = distancia( PIN_0, PIN_1);

    	if(dist <= 20){
    		GPIO_Write_Pin(GPIOA, PIN_2, HIGH);
    		Delay_ms(50);
    		GPIO_Write_Pin(GPIOA, PIN_2, LOW);
    		Delay_ms(50);
    	} else if(dist > 20 && dist < 40){
    		GPIO_Write_Pin(GPIOA, PIN_2, HIGH);
    		Delay_ms(300);
    		GPIO_Write_Pin(GPIOA, PIN_2, LOW);
    		Delay_ms(300);
    	} else if(dist >= 40 && dist <= 50){
    		GPIO_Write_Pin(GPIOA, PIN_2, HIGH);
    		Delay_ms(600);
    		GPIO_Write_Pin(GPIOA, PIN_2, LOW);
    		Delay_ms(600);

    	}
    	else if(dist >= 50){
    	}
    }
}
*/
void Display_Write(uint8_t value)
{
    // Desliga os 7 segmentos
    GPIOD->BSRR = (0x7F << 16);

    // Escreve a nova máscara
    GPIOD->BSRR = value;
}



void valuesKey(uint8_t i, uint8_t j)
    {

	uint8_t values[16] = {
	    0b0100000, // 0
	    0b1111001, // 1
	    0b0100100, // 2
	    0b0110000, // 3
	    0b0011001, // 4
	    0b0010010, // 5
	    0b0000010, // 6
	    0b1111000, // 7
	    0b0000000, // 8
	    0b0010000, // 9
	    0b0001000, // A
	    0b0000011, // B
	    0b1000110, // C
	    0b0100001, // D
	    0b0000110, // E
	    0b0001110  // F
	};
        // 1 2 3
        // 4 5 6
        // 7 8 9
        if (j < 3 && i < 3)
        {
            GPIOD->BSRR = (0b1111111 << 16);
            GPIOD->BSRR = values[i * 3 + j + 1];

            return;
        }

        // Última linha: * 0 # D
        if (i == 3)
        {
            switch (j)
            {
                case 0:
                    // *
                    GPIOD->BSRR = (0b1111111 << 16);
                    GPIOD->BSRR = values[14];
                    break;

                case 1:
                    // 0
                    GPIOD->BSRR = (0b1111111 << 16);
                    GPIOD->BSRR = values[0];
                    break;

                case 2:
                    // #
                    GPIOD->BSRR = (0b1111111 << 16);
                    GPIOD->BSRR = values[15];
                    break;

                case 3:
                    // D
                    GPIOD->BSRR = (0b1111111 << 16);
                    GPIOD->BSRR = values[13];
                    break;
            }
        }

        // Última coluna: A B C D
        else if (j == 3)
        {
            switch (i)
            {
                case 0:
                    // A
                    GPIOD->BSRR = (0b1111111 << 16);
                    GPIOD->BSRR = values[10];
                    break;

                case 1:
                    // B
                    GPIOD->BSRR = (0b1111111 << 16);
                    GPIOD->BSRR = values[11];
                    break;

                case 2:
                    // C
                    GPIOD->BSRR = (0b1111111 << 16);
                    GPIOD->BSRR = values[12];
                    break;

                case 3:
                    // D
                    GPIOD->BSRR = (0b1111111 << 16);
                    GPIOD->BSRR = values[13];
                    break;
            }
        }
    }


void questao19()
{



    // -----------------------------------------------------
    // Função para descobrir qual valor deve aparecer
    // -----------------------------------------------------



    // -----------------------------------------------------
    // Inicialização
    // -----------------------------------------------------

    GPIO_Clock_Enable(GPIOA);
    GPIO_Clock_Enable(GPIOD);


    // -----------------------------------------------------
    // PA0 - PA3 = LINHAS
    // -----------------------------------------------------

    for (uint8_t i = 0; i < 4; i++)
    {
        GPIO_Pin_Mode(GPIOA, i, OUTPUT);

        GPIO_Output_Type(
            GPIOA,
            i,
            OPEN_DRAIN
        );

        GPIO_Resistor_Enable(
            GPIOA,
            i,
            PULL_UP
        );

        // Inicialmente todas as linhas desligadas
        GPIO_Write_Pin(
            GPIOA,
            i,
            HIGH
        );
    }


    // -----------------------------------------------------
    // PA4 - PA7 = COLUNAS
    // -----------------------------------------------------

    for (uint8_t i = 0; i < 4; i++)
    {
        GPIO_Pin_Mode(
            GPIOA,
            4 + i,
            INPUT
        );

        GPIO_Resistor_Enable(
            GPIOA,
            4 + i,
            PULL_UP
        );
    }


    // -----------------------------------------------------
    // DISPLAY
    // -----------------------------------------------------

    for (uint8_t i = 0; i < 7; i++)
    {
        GPIO_Pin_Mode(
            GPIOD,
            i,
            OUTPUT
        );
    }


    // -----------------------------------------------------
    // VARREDURA
    // -----------------------------------------------------

    while (1)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            // Todas as linhas em HIGH
            GPIO_Write_Pin(GPIOA, PIN_0, HIGH);
            GPIO_Write_Pin(GPIOA, PIN_1, HIGH);
            GPIO_Write_Pin(GPIOA, PIN_2, HIGH);
            GPIO_Write_Pin(GPIOA, PIN_3, HIGH);

            // Linha atual em LOW
            GPIO_Write_Pin(
                GPIOA,
                i,
                LOW
            );

            Delay_us(10);


            // Lê as quatro colunas
            for (uint8_t j = 0; j < 4; j++)
            {
                /*
                 * Como as colunas possuem PULL-UP:
                 *
                 * nenhuma tecla = HIGH
                 * tecla pressionada = LOW
                 */
                if (GPIO_Read_Pin(
                        GPIOA,
                        4 + j
                    ) == LOW)
                {
                    valuesKey(i, j);

                    Delay_us(20000);
                }
            }
        }
    }
}

void questao20(){
    GPIO_Clock_Enable(GPIOA);
    GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT); // trigger
    GPIO_Pin_Mode(GPIOA, PIN_1, INPUT); // echo
    GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT); // buzzer
    USART1_Init();

    while(1){
    	int dist = distancia( PIN_0, PIN_1);
		printf("distancia em cm: %d\n", dist);
		Delay_ms(200)

    }
}



#endif /*ATVVAVALIATIVAONE_H_*/
