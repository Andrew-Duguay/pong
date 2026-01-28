
#ifndef SSD1306_H        // If this macro is NOT defined
#define SSD1306_H        // Then define it

#include <stdint.h>
#include "ssd1306_config.h"

void display();

void displayPartial(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

void clearDisplay();

void OLED_display_init();

void normalDisplay();

void inverseDisplay();

void setContrast(uint8_t contrast);

void resumeDisplay();

void displayON();

void displayOFF();

void allPixelsOn();

void setVerticalAddressingMode();

void setHorizontalAddressingMode();

void setPageAddressingRange(uint8_t min, uint8_t max);

void setColumnAddressingRange(uint8_t min, uint8_t max);

void sendCommand(uint8_t * dataStream, int size);

void sendData(uint8_t* dataStream, int size);

void drawPixel(int x, int y);

void erasePixel(int x, int y);

void drawChar(int x, int y, char c, int font);

void drawString(int x, int y, char* str, int font);

void drawStringPartial(int x, int y, char* str, int font);

#endif
