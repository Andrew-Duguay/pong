#include "i2c.h"
#include "stm32f4xx.h"
#define I2C1EN 			(1U<<21)
#define GPIOBEN			(1U<<1)
#define I2C1_CR1_PE		(1U<<0)
#define I2C1_CR1_START	(1U<<8)
#define I2C1_CR1_STOP	(1U<<9)
#define I2C1_CR1_ACK	(1U<<10)
#define I2C1_SR1_SB		(1U<<0)
#define I2C1_SR1_ADDR	(1U<<1)
#define I2C1_SR1_BTF	(1U<<2)
#define I2C1_SR1_RXNE	(1U<<6)
#define I2C1_SR1_TXE 	(1U<<7)
#define I2C1_SR2_BUSY	(1U<<1)
#define I2C_100KHZ				80
#define STD_MODE_MAX_RISE_TIME 	17

void I2C1_init(){
	//PB8 for SCL, PB9 for SDA
	RCC->AHB1ENR |= GPIOBEN;

	GPIOB->MODER &=~(1U<<16);		//set alt func mode
	GPIOB->MODER |= (1U<<17);
	GPIOB->MODER &=~(1U<<18);
	GPIOB->MODER |= (1U<<19);

	GPIOB->AFR[1] &=~(1U<<0);		//Set PB8 to AF4
	GPIOB->AFR[1] &=~(1U<<1);
	GPIOB->AFR[1] |= (1U<<2);
	GPIOB->AFR[1] &=~(1U<<3);
	GPIOB->AFR[1] &=~(1U<<4);		//Set PB9 to AF4
	GPIOB->AFR[1] &=~(1U<<5);
	GPIOB->AFR[1] |= (1U<<6);
	GPIOB->AFR[1] &=~(1U<<7);

	GPIOB->OTYPER |= (1U<<8);		//set open drain output type
	GPIOB->OTYPER |= (1U<<9);

	GPIOB->PUPDR |= (1U<<16);		//enable pull-up resistors
	GPIOB->PUPDR &=~(1U<<17);
	GPIOB->PUPDR |= (1U<<18);
	GPIOB->PUPDR &=~(1U<<19);

	RCC->APB1ENR |= I2C1EN;

	I2C1->CR1 |= (1U<<15);			//enter reset mode
	I2C1->CR1 &=~(1U<<15);			//exit reset mode

	I2C1->CR2 |= (1U<<4);			//Set peripheral clock frequency
	I2C1->CCR = I2C_100KHZ;			//Set I2c to std mode, 100KHz clock
	I2C1->TRISE = STD_MODE_MAX_RISE_TIME;	//rise time?
	I2C1->CR1 |= I2C1_CR1_PE;		//peripheral enable
}

void I2C1_read(uint8_t slave_add, uint8_t mem_add, uint8_t* data){
	volatile int temp;
	while (I2C1->SR2 & I2C1_SR2_BUSY){}		//wait until no longer busy
	I2C1->CR1 |= I2C1_CR1_START;			//initiate start
	while(!(I2C1->SR1 & I2C1_SR1_SB)){}		//wait until start flag is set
	I2C1->DR = slave_add << 1;				//slave address + 0(write)
	while(!(I2C1->SR1 & I2C1_SR1_ADDR)){}	//Wait until address flag set
	temp = I2C1->SR2;						//Clear Address flag
	(void)temp;
	I2C1->DR = mem_add;						//send mem addr to read data from
	while(!(I2C1->SR1 & I2C1_SR1_TXE)){}	//wait until transmitter empty
	I2C1->CR1 |= I2C1_CR1_START;			//generate RESTART
	while(!(I2C1->SR1 & I2C1_SR1_SB)){}		//wait until start flag is set
	I2C1->DR = slave_add << 1 | 1;			//slave address + 1(read)
	while(!(I2C1->SR1 & I2C1_SR1_ADDR)){}	//Wait until address flag set
	I2C1->CR1 &=~I2C1_CR1_ACK;				//Disable ACK
	temp = I2C1->SR2;						//Clear Address flag
	(void)temp;
	I2C1->CR1 |= I2C1_CR1_STOP;				//gen stop after data recieved
	while(!(I2C1->SR1 & I2C1_SR1_RXNE)){}	//Wait until RXNE flag set
	*data++ = I2C1->DR;
}

void I2C1_burstRead(uint8_t slave_add, uint8_t mem_add, int n, uint8_t* data){
	volatile int temp;
	while (I2C1->SR2 & I2C1_SR2_BUSY){}		//wait until no longer busy
	I2C1->CR1 |= I2C1_CR1_START;			//initiate start
	while(!(I2C1->SR1 & I2C1_SR1_SB)){}		//wait until start flag is set
	I2C1->DR = slave_add << 1;				//slave address + 0(write)
	while(!(I2C1->SR1 & I2C1_SR1_ADDR)){}	//Wait until address flag set
	temp = I2C1->SR2;						//Clear Address flag
	(void)temp;
	while(!(I2C1->SR1 & I2C1_SR1_TXE)){}	//wait until transmitter empty
	I2C1->DR = mem_add;						//send mem addr to read data from
	while(!(I2C1->SR1 & I2C1_SR1_TXE)){}	//wait until transmitter empty
	I2C1->CR1 |= I2C1_CR1_START;			//generate RESTART
	while(!(I2C1->SR1 & I2C1_SR1_SB)){}		//wait until start flag is set
	I2C1->DR = slave_add << 1 | 1;			//slave address + 1(read)
	while(!(I2C1->SR1 & I2C1_SR1_ADDR)){}	//Wait until address flag set
	temp = I2C1->SR2;						//Clear Address flag
	(void)temp;
	I2C1->CR1 |= I2C1_CR1_ACK;				//Enable ACK
	while(n > 0U){
		if(n == 1){
			I2C1->CR1 &=~I2C1_CR1_ACK;				//disable ACK
			I2C1->CR1 |= I2C1_CR1_STOP;				//gen stop after data recieved
			while(!(I2C1->SR1 & I2C1_SR1_RXNE)){}	//Wait until RXNE flag set
			*data++ = I2C1->DR;
			break;
		}
		else{
			while(!(I2C1->SR1 & I2C1_SR1_RXNE)){}	//Wait until RXNE flag set
			*data++ = I2C1->DR;
			n--;
		}
	}
	I2C1->CR1 |= I2C1_CR1_STOP;				//gen stop after data recieved
}

void I2C1_burstWrite(uint8_t slave_add, uint8_t mem_add, int n, uint8_t* data){
	volatile int temp;
	while (I2C1->SR2 & I2C1_SR2_BUSY){}		//wait until no longer busy
	I2C1->CR1 |= I2C1_CR1_START;			//initiate start
	while(!(I2C1->SR1 & I2C1_SR1_SB)){}		//wait until start flag is set
	I2C1->DR = slave_add << 1;				//slave address + 0(write)
	while(!(I2C1->SR1 & I2C1_SR1_ADDR)){}	//Wait until address flag set
	temp = I2C1->SR2;						//Clear Address flag
	(void)temp;
	while(!(I2C1->SR1 & I2C1_SR1_TXE)){}	//wait until transmitter empty
	I2C1->DR = mem_add;						//send mem addr to write data from
	for(int i = 0; i < n; i++){
		while(!(I2C1->SR1 & I2C1_SR1_TXE)){}	//wait until transmitter empty
		I2C1->DR = *data++;						//TX data
	}
	while(!(I2C1->SR1 & I2C1_SR1_BTF)){}
	I2C1->CR1 |= I2C1_CR1_STOP;
}

