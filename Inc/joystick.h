#ifndef JOYSTICK_H
#define JOYSTICK_H

//initializes adc to read joystick
void joystick_init();

// initialize GPIO pin to read button
void pause_button_init();

// Converts signal from joy stick into value from 0 to 49
int readJoy();

// returns 1 if button was pressed
int button_pressed();

#endif // JOYSTICK_H
