#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"
#include <stdint.h>

void I2C1_init();

void I2C1_read(uint8_t slave_add, uint8_t mem_add, uint8_t* data);

void I2C1_burstRead(uint8_t slave_add, uint8_t mem_add, int n, uint8_t* data);

void I2C1_burstWrite(uint8_t slave_add, uint8_t mem_add, int n, uint8_t* data);

#endif
