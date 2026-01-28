/*
 * adc.h
 *
 *  Created on: Jan 7, 2025
 *      Author: andre
 */

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>
#define SR_EOC			(1U<<1) //Detect ADC1 EOC flag for interrupt read

void pa1_adc_interrupt_init(void);
void temp_sensor_init(void);
void start_conversion(void);
void pa1_adc_init(void);
uint32_t adc_read(void);
uint32_t temp_sensor_read();


#endif /* ADC_H_ */
