#ifndef SSD1306_CONFIG_H_
#define SSD1306_CONFIG_H_

#define NUM_INIT_COMMANDS   18
#define SCREEN_WIDTH        128 //Column-resolution
#define SCREEN_HEIGHT       64  //Row resolution
#define NUM_PAGES           8   //pages used in addressing mode
#define BITS_PER_PAGE		8
#define NUMBER_OF_BYTES		(8 * 128)
#define SLAVE_ADDRESS       (0x78 >> 1)
#define COMMAND_CTRL_BYTE   0x00
#define DATA_CTRL_BYTE      0x40

#endif //SSD1306_CONFIG_H_
