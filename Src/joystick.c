#include "adc.h"
#include "timer.h"
#include <stdint.h>
#include "stm32f446xx.h"

void joystick_init(){
	pa1_adc_init();
	start_conversion();
}

void pause_button_init(){
	GPIOA->MODER &= ~(1U << 1);	//set PA0 to input mode
	GPIOA->MODER &= ~(1U << 0);
	GPIOA->PUPDR &= ~(1U << 1);	//Set PA0 to pull-up resistor
	GPIOA->PUPDR |= (1U << 0); //pg 190
}

// Converts signal from joy stick into value from 0 to 49
int readJoy(){
	return adc_read();
}

// returns 1 if button was pressed
int button_pressed(){
	//switch active low. sw = 0 when pressed
	int sw = GPIOA->IDR & 1U;
	if(sw != 0) return 0;
	delay(30);
	while (!sw){
		sw = GPIOA->IDR & 1U;
	}
	delay(30);
	return 1;
}
