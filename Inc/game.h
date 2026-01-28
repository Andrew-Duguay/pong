/*
 * game.h
 *
 *  Created on: Jan 1, 2026
 *      Author: andre
 */

#ifndef GAME_H_
#define GAME_H_
#include <stdint.h>

#define PADDLE_WIDTH 30
#define PADDLE_HEIGHT 3
#define PADDLE_DISPLAY_HEIGHT 2
#define PADDLE_YPOS 60
#define PADDLE_DISPLAY_YPOS 61
#define BALL_RADIUS 2
#define FP_SHIFT 8
#define FP_SCALE (1U << 8)

typedef struct {
	int x_left; //left edge of paddle
	int x_right; //right edge of paddle
	int x_leftbound; // leftmost pixel x_left can go
	int x_rightbound;// rightmost pixel x_left can go
	int dir; // left = -1, right = 1, still = 0
	int dx; //speed paddle will move
}Paddle;

typedef struct {
	int32_t x, y;
	int32_t dx, dy;
	int dir_x, dir_y;
	int r;
	int num_pings;
	int LUT_index;
}Ball;
//initializes peripherals and interrupt. does NOT enable NVIC interrupt
void game_init();

//enable NVIC interrupt and initializes paddle and ball, starting the game
void game_start();

//disables NVIC interrupt, stopping the game
void game_stop();

//Stops game and triggers game over screen
void gameOver(Ball* b);

//converts integers to ascii and appends to end of string
void getScore(int points, char* score);

// pauses game if switch toggled
// returns 1 if game is paused, 0 if running.
int check_for_pause();

//returns 1 if game is running, 0 if over
int is_game_running();

void gameplay_interrupt_init();

void TIM3_IRQHandler();

void reset_paddle();

// Logic to check direction and move paddle
void movePaddle(Paddle* p);

void reset_ball();

int moveBall(Ball* b, Paddle *p);


#endif /* GAME_H_ */
