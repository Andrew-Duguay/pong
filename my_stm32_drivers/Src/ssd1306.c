#include "ssd1306.h"
#include "ssd1306_config.h"
#include "ssd1306_fonts.h"
#include "i2c.h"
#include "graphics.h"

uint8_t display_buffer[NUM_PAGES * SCREEN_WIDTH] = {0};

void display(){
  setHorizontalAddressingMode();
  resumeDisplay();
  setColumnAddressingRange(0, (SCREEN_WIDTH-1));
  setPageAddressingRange(0, (NUM_PAGES - 1));
  sendData(display_buffer, NUMBER_OF_BYTES);
}

void displayPartial(uint8_t x, uint8_t y, uint8_t width, uint8_t height){
	setHorizontalAddressingMode();
	resumeDisplay();
	//Calculate page ranges based on pixel ranges given
	uint8_t page_min = y / 8;
	uint8_t page_max = (y + height - 1) / 8;
	uint8_t pages_to_send = page_max - page_min + 1;
	setColumnAddressingRange(x, x + width - 1);
	setPageAddressingRange(page_min, page_max);

	for (int i = 0; i < pages_to_send; i++) {
		int currentPage = page_min + i;

		// Calculate the memory address of the start of this specific row in your buffer
		// Offset = (Page number * 128) + x_offset
		uint8_t* pageStartPtr = &display_buffer[(currentPage * 128) + x];

		// Send only the 'width' number of bytes for this row
		sendData(pageStartPtr, width);
	}
}

void clearDisplay(){      //does NOT clear RAM, only buffer. Ram updates on display()
  for(int i = 0; i < NUMBER_OF_BYTES; i++){
      display_buffer[i] = 0;
  }
  display();
}

void OLED_display_init(){
	I2C1_init();
	displayOFF();
	sendCommand((uint8_t[]){0xA8, 0x3F}, 2);      //Set mux ratio
	sendCommand((uint8_t[]){0xD3, 0x00}, 2);      //set display offset
	sendCommand((uint8_t[]){0x40}, 1);            //Set Display start line
	sendCommand((uint8_t[]){0xA0}, 1);            //Set Segment Re-map, A1 <-> A0 to change HOR reflection
	sendCommand((uint8_t[]){0xC0}, 1);            //Set COM output scan direction, C0 <-> C8 to change VERT reflection
	sendCommand((uint8_t[]){0xDA, 0x12 /*0x02*/}, 2);      //Set COM pin HW config
	setContrast(0x7F);
	resumeDisplay();                              //Disable allPixelsOn()
	normalDisplay();                              //non-inverted
	sendCommand((uint8_t[]){0xD5, 0x80}, 2);      //Set Osc Freq
	sendCommand((uint8_t[]){0x8D, 0x14}, 2);      //Enable Charge Pump regulator
	setHorizontalAddressingMode();
	displayON();
}

void normalDisplay(){     //un-invert display
	uint8_t command[] = {0xA6};
	sendCommand(command, 1);
}

void inverseDisplay(){     //invert display
	uint8_t command[] = {0xA7};
	sendCommand(command, 1);
}

void setContrast(uint8_t contrast){
	uint8_t command[] = {0x81, contrast};
	sendCommand(command, 2);
}

void resumeDisplay(){       //Display from GDDRAM
	uint8_t command[] = {0xA4};
	sendCommand(command, 1);
}

void displayON(){         //Wake display from sleep mode
	uint8_t command[] = {0xAF};
	sendCommand(command, 1);
}

void displayOFF(){        //sleep mode
	uint8_t command[] = {0xAE};
	sendCommand(command, 1);
}

void allPixelsOn(){
	uint8_t command[] = {0xA5};
    sendCommand(command, 1);
}

void setVerticalAddressingMode(){
	sendCommand((uint8_t[]){ 0x20, 0x01 }, 2);
}

void setHorizontalAddressingMode(){
	sendCommand((uint8_t[]){ 0x20, 0x00 }, 2);    //set horizontal addressing mode
}

void setPageAddressingRange(uint8_t min, uint8_t max){
	uint8_t command[] = {0x22, min, max};
	sendCommand(command, 3);
}

void setColumnAddressingRange(uint8_t min, uint8_t max){
	uint8_t command[] = {0x21, min, max};
	sendCommand(command, 3);
}

void sendCommand(uint8_t * dataStream, int size){
	I2C1_burstWrite((uint8_t)SLAVE_ADDRESS, (uint8_t)COMMAND_CTRL_BYTE, size, dataStream);
}

void sendData(uint8_t* dataStream, int size){
	I2C1_burstWrite((uint8_t)SLAVE_ADDRESS, (uint8_t)DATA_CTRL_BYTE, size, dataStream);
}

void drawPixel(int x, int y){
  if (x >= SCREEN_WIDTH || x < 0 || y >= SCREEN_HEIGHT || y < 0){
    return;
  }
  uint8_t bit_num = y % BITS_PER_PAGE;
  int page = y/NUM_PAGES;
  uint8_t mask = 1 << (bit_num);
  int byte_index = page * SCREEN_WIDTH + x;
  display_buffer[byte_index] |= mask;
}

void erasePixel(int x, int y){
  if (x >= SCREEN_WIDTH || x < 0 || y >= SCREEN_HEIGHT || y < 0){
    return;
  }
  char bit_num = y % BITS_PER_PAGE;
  int page = y/NUM_PAGES;
  char mask = ~(1 << (bit_num));
  int byte_index = page * SCREEN_WIDTH + x;
  display_buffer[byte_index] &= mask;
}

void drawChar(int x, int y, char c, int font) {
	const uint8_t* fontArr;
	if(font == 1){
		fontArr = font1_6x8;
	}else{
		fontArr = font2_8x16;
	}
    uint8_t width      = fontArr[1];
    uint8_t height     = fontArr[2];
    uint8_t start_char = fontArr[3];

    if (c < start_char) return;

    uint8_t bytes_per_col = (height + 7) / 8;
    int font_idx = 4 + (c - start_char) * (width * bytes_per_col);

    for (int p = 0; p < bytes_per_col; p++) {
        for (int col = 0; col < width; col++) {
            uint8_t byte = fontArr[font_idx + (p * width) + col];

            for (int bit = 0; bit < 8; bit++) {
                if (byte & (1 << bit)) {
                    drawPixel(x + col, y + (p * 8) + bit);
                } else {
                    erasePixel(x + col, y + (p * 8) + bit);
                }
            }
        }
    }
}

void drawString(int x, int y, char* str, int font) {
	const uint8_t* fontArr;
	if(font == 1){
		fontArr = font1_6x8;
	}else{
		fontArr = font2_8x16;
	}
    // Get the character width from the font header (6px for your 6x8 font)
    uint8_t width = fontArr[1];

    while (*str) {
        // Prevent drawing off the right edge of the screen
        if (x + width > 128) break;

        drawChar(x, y, *str++, font);

        // Advance X by the font width plus 1 pixel for kerning (spacing)
        x += (width + 1);
    }
}

/**
 * @brief Helper to draw a string and immediately update only that section
 * of the hardware display.
 */
void drawStringPartial(int x, int y, char* str, int font) {
	const uint8_t* fontArr;
	if(font == 1){
		fontArr = font1_6x8;
	}else{
		fontArr = font2_8x16;
	}
    int start_x = x;
    uint8_t width = fontArr[1];
    uint8_t height = fontArr[2];
    int char_count = 0;

    // First, write to the local display_buffer
    char* ptr = str;
    while (*ptr) {
        drawChar(x, y, *ptr++, font);
        x += (width + 1);
        char_count++;
    }

    // Calculate total width of the string drawn
    // (Width + 1 pixel kerning) * count
    int total_width = char_count * (width + 1);

    // Now perform a partial hardware update to the SSD1306
    displayPartial(start_x, y, total_width, height);
}

void reverseString(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itos(int num, char* str) {
    int i = 0;
    int isNegative = 0;
    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    // Handle negative numbers
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }
    // Process individual digits
    while (num != 0) {
        int rem = num % 10;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / 10;
    }
    // Append negative sign
    if (isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    reverseString(str, i);
    return str;
}
