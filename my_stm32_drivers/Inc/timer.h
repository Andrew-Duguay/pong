#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
//simple timer driver for creating timestamps and non-blocking delay(ms)

void timer_init();

//returns the present timer stamp in us
uint32_t micros();

//calls micros() and converts to ms
uint32_t millis();

//delays for ms
void delay(int ms);

#endif //TIME_H_
