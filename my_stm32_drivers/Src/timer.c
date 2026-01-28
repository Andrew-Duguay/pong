#include "stm32f446xx.h"
#include <stdint.h>

static int timer_is_initialized = 0;

void timer_init(){
	if(timer_is_initialized){return;}
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	//enable APB1 clock access to timer 2
	TIM2->PSC = 15;						//16MHz/(15+1) = 1MHz
	TIM2->EGR |= TIM_EGR_UG;			// 5. Clear the UIF flag triggered by the UG bit above
	TIM2->SR &= ~TIM_SR_UIF;			//Clear flag from UG bit above
	TIM2->ARR = 0xFFFFFFFF;				//Large range of possible delays
	TIM2->CR1 |= TIM_CR1_CEN;
	timer_is_initialized = 1;
}

uint32_t micros(){
	timer_init();
	return TIM2->CNT;
}

uint32_t millis(){
	return micros()/1000;
}

void delay(uint32_t ms){
	uint32_t start = micros();
	uint32_t wait_time = ms*1000;
	while( ( micros()-start) < wait_time);
}


