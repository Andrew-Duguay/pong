#include <graphics.h>
#include <ssd1306.h>
#include "stm32f446xx.h"
#include <stdio.h>
#include <stdint.h>
#include "timer.h"
#include "game.h"
#include "joystick.h"

void print_opening_screen();
void print_game_start();

int main(void)
{

	OLED_display_init();
	game_init();

	//DBG
	while(1){
		print_opening_screen();
		while(!button_pressed()); //wait til button is pressed to start game
		print_game_start();
		game_start();
		while(is_game_running());
		while(!button_pressed());
	};
}

void print_opening_screen(){
	clearDisplay();
	drawStringPartial(6,1,"Andrew's pong",2);
	drawStringPartial(42,20,"game!",2);
	drawStringPartial(1, 46,"Click the joystick",1);
	drawStringPartial(37,55,"to start",1);
}

void print_game_start(){
	clearDisplay();
	display();
	drawStringPartial(30,10,"LET",2);
	delay(200);
	drawStringPartial(70,10,"THE",2);
	delay(200);
	drawStringPartial(14,35,"GAME",2);
	delay(200);
	drawStringPartial(60,35,"BEGIN!",2);
	delay(200);
	clearDisplay();
}






