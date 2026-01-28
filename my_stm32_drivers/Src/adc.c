#include "stm32f446xx.h"
#include "adc.h"
#include <stdint.h>

#define RCC_APB2ENR_ADC1EN			(1U<<8)	//RCC_APB1ENR
#define CR2_SWSTART					(1U<<30)//Start ADC1 conversion
#define CR2_CONT					(1U<<1) //Configure ADC1 for continuos conversion
#define SQR1_SEG_LEN_1				(0x00)	//Configure ADC1 sequence length (Number of channels) to 1
#define CR2_ADON					(1U<<0) //Power on ADC1 module
//ADC1 Channel 1
#define GPIOAEN						(1U<<0) //RCC_AHB1ENR
#define SQR3_CH1					(1U<<0)	//Set ADC1 Channel 1 as 1st in sequence
#define CR1_EOCIE					(1U<<5) //Enable EOC interrupt for ADC1
//Temp Sensor
#define ADC_CCR_TEMPSEN				(1U<<23) //Enable Temp Sensor in ADC registers (not ADC1)
#define SQR3_CH18					(18U<<0) //Set ADC1 Channel 18 as 1st in sequence
#define SMPR1_CH18					(1U<<24)//ADC1 channel 18 sampling time set to 15 cycles

/*
 * Below functions used to configure a single channel for ADC conversion,
 * triggered by interrupt.
 * PA1 is ADC pin
 * Raw values only.
 */
void pa1_adc_init(void){
	RCC->AHB1ENR |= GPIOAEN;				//enable clk access to adc pin port (A)
	GPIOA->MODER |= (1U<<2);				//setPA1 to analog
	GPIOA->MODER |= (1U<<3);
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;		//enable clock access to ADC1
	//ADC1->CR1 |= CR1_EOCIE; 				//Enable ADC end of conversion interrupt
	//NVIC_EnableIRQ(ADC_IRQn); 				//Enable ADC interrupt in NVIC
	ADC1->SQR3 = SQR3_CH1; 					//Set channel 1 as 1st conversion in sequence
	ADC1->SQR1 = SQR1_SEG_LEN_1; 			//Set num channels to sample to 1
	ADC1->CR2 |= CR2_ADON;					//Enable ADC module
}

void start_conversion(void){
	 ADC1->CR2 |= CR2_SWSTART;	//start signal
	 ADC1->CR2 |= CR2_CONT;		//continuos conv mode
}

uint32_t adc_read(void){
	//wait for conversion to complete (status register)
	while(!(SR_EOC & ADC1->SR)){}
	//read result
	return ADC1->DR;
}

/*
 * Below functions initialize temp sensor as only ADC channel to sample
 * Converts raw read to F
 * Readings must be manually read with temp_sensor_read()
 */
void temp_sensor_init(void){
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;	//enable clock access to ADC1
	ADC->CCR |= ADC_CCR_TEMPSEN;		//Enable temperature sensor
	ADC1->SQR3 = SQR3_CH18;				//Enable ADC Channel 18
	ADC1->SQR1 = SQR1_SEG_LEN_1;		//Ensure length bits are 0000 (length of 1)
	ADC1->SMPR1 |= SMPR1_CH18;			//Configure sampling time
	ADC1->CR2 |= CR2_ADON;				//Power on and start ADC module
	ADC1->CR2 |= CR2_SWSTART;			//start signal
	ADC1->CR2 |= CR2_CONT;				//continuos conv mode
}

uint32_t temp_sensor_read(){
	while(!(SR_EOC & ADC1->SR)){}
	float temp = (float)ADC1->DR / 4095;
	temp = (temp)*3.3;	//convert raw value
	temp = ((temp - 0.76) / 0.0025);
	temp = temp + 25;		//calculate in C
	return  (temp)*1.8 + 32;				//convert to F
}
