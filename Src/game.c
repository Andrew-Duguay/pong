#include "game.h"
#include "timer.h"
#include "ssd1306.h"
#include "graphics.h"
#include "joystick.h"
#include "stm32f446xx.h"

//LUT for variable trajectory (from paddle)
const int32_t bounce_lut[11][2] = {
		{-241, -88},  // 0: Sharp Left
		{-215, -138}, // 1
		{-181, -181}, // 2: Mid Left
		{-138, -215}, // 3
		{-88,  -241}, // 4: Shallow Left
		{0,   -256}, // 5: Vertical (Center)
		{88,  -241}, // 6: Shallow Right
		{138, -215}, // 7
		{181, -181}, // 8: Mid Right
		{215, -138}, // 9
		{241, -88}   // 10: Sharp Right
};

// mutex to prevent graphics update
volatile int paused = 0;
// flag for game state
volatile int game_running = 0;
// Objects for the paddle and ball
static Paddle thePaddle = {
	.x_left = (SCREEN_WIDTH - PADDLE_WIDTH) / 2,
	.x_right = (SCREEN_WIDTH + PADDLE_WIDTH) / 2,
	.x_leftbound = SCREEN_WIDTH - PADDLE_WIDTH,
	.x_rightbound = SCREEN_WIDTH,
	.dx = 2
};
static Ball theBall = {
	.r = BALL_RADIUS,
	.x = (SCREEN_WIDTH/2) << FP_SHIFT, //top = y + r
	.y = (PADDLE_YPOS - BALL_RADIUS - 1) << FP_SHIFT,
	.LUT_index = 2,
	.dx = bounce_lut[2][0],
	.dy = bounce_lut[2][1],
	.dir_x = 1,
	.dir_y = -1,
	.num_pings = 0
};

//initializes peripherals and interrupt. does NOT enable NVIC interrupt
void game_init(){
	int start_x = theBall.x >> FP_SHIFT;
	int start_y = theBall.y >> FP_SHIFT;
	clearDisplay();
	fillRect(thePaddle.x_left, PADDLE_DISPLAY_YPOS, PADDLE_WIDTH, PADDLE_DISPLAY_HEIGHT);
	drawCircle(start_x, start_y, theBall.r);
	display();
	joystick_init();
	gameplay_interrupt_init();
	pause_button_init();
}

//enable NVIC interrupt, starting the game
void game_start(){
	game_running = 1;
	reset_paddle();
	reset_ball();
	NVIC_EnableIRQ(TIM3_IRQn); 			//Enable ADC interrupt in NVIC
}

//disables NVIC interrupt, stopping the game
void game_stop(){
	game_running = 0;
	NVIC_DisableIRQ(TIM3_IRQn);
}

//Stops game and triggers game over screen
void gameOver(Ball* b){
	game_stop();
	char score_str[10] = "SCORE:   ";
	getScore(b->num_pings, score_str);
	clearDisplay();
	drawStringPartial(23,2,"GAME OVER",2);
	delay(1000);
	drawStringPartial(29,20,score_str,1);
	drawStringPartial(1, 46,"Click the joystick",1);
	drawStringPartial(19,55,"to play again",1);
}

//converts integers to ascii and appends to end of string
void getScore(int points, char* score){
	uint8_t b = (points / 10) % 10;
	uint8_t c = points % 10;
	score[7] = b + '0';
	score[8] = c + '0';
	score[9] = '\0';
}

// pauses game if switch toggled
// returns 1 if game is paused, 0 if running.
int check_for_pause(){
	// No pause button activity, then return current state
	if (!button_pressed()) return paused;
	// Pause button was pressed if here is reached
	// If un-pause -> pause then print pause screen
	if (paused == 0) {
		paused = 1;			// Acts as mutex for I2C comms on interrupt
		clearDisplay();
		drawStringPartial(41, 26, "PAUSED", 2);
	} else { // pause -> un-pause then restore game screen
		clearDisplay();
		drawCircle(theBall.x >> FP_SHIFT, theBall.y >> FP_SHIFT, theBall.r);
		fillRect(thePaddle.x_left, PADDLE_DISPLAY_YPOS, PADDLE_WIDTH, PADDLE_DISPLAY_HEIGHT);
		display();
		paused = 0;			// Frees mutex up
	}
	return paused;
}

//returns 1 if game is running, 0 if over
int is_game_running(){
	return game_running;
}

// Initializes adc to read joy stick potentiometer input


void gameplay_interrupt_init(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//enable APB1 clock access to timer 2
	TIM3->PSC = 15;						//16MHz/(15+1) = 1MHz
	TIM3->EGR |= TIM_EGR_UG;			// 5. Clear the UIF flag triggered by the UG bit above
	TIM3->SR &= ~TIM_SR_UIF;			//Clear flag from UG bit above
	TIM3->ARR = 15999;					//16ms sample rate (~60fps)
	TIM3->DIER |= 1U;					//Enable upd. int. UIE
	TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3_IRQHandler(){
	//do stuff on update event
	if(TIM3->SR & TIM_SR_UIF){		//check for update event
		TIM3->SR &= ~TIM_SR_UIF;	//clear interrupt flag
		if(check_for_pause()) return;
		movePaddle(&thePaddle);
		moveBall(&theBall, &thePaddle);
		if(theBall.num_pings == 99){
			gameOver(&theBall);
		}
	}
}

void reset_paddle(){
	clearRect(thePaddle.x_left, PADDLE_DISPLAY_YPOS, PADDLE_WIDTH, PADDLE_DISPLAY_HEIGHT);
	displayPartial(thePaddle.x_left, PADDLE_DISPLAY_YPOS, PADDLE_WIDTH, PADDLE_DISPLAY_HEIGHT);
	thePaddle.x_left = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
	thePaddle.x_right = (SCREEN_WIDTH + PADDLE_WIDTH) / 2;
	thePaddle.x_leftbound = SCREEN_WIDTH - PADDLE_WIDTH;
	thePaddle.x_rightbound = SCREEN_WIDTH;
	thePaddle.dx = 2;
	fillRect(thePaddle.x_left, PADDLE_DISPLAY_YPOS, PADDLE_WIDTH, PADDLE_DISPLAY_HEIGHT);
	displayPartial(thePaddle.x_left, PADDLE_DISPLAY_YPOS, PADDLE_WIDTH, PADDLE_DISPLAY_HEIGHT);
}

// Logic to check direction and move paddle
void movePaddle(Paddle* p){
	int dir = readJoy() / 82;
	int dx = p->dx;
	if(dir > 25){
		p->x_left -= dx;	//Move paddle location
		if(p->x_left < 0) p->x_left = 0;	//snap back if over edge
		p->x_right = p->x_left + PADDLE_WIDTH; //update right coordinate
		clearRect(p->x_right, PADDLE_DISPLAY_YPOS, dx, PADDLE_DISPLAY_HEIGHT);
		fillRect(p->x_left, PADDLE_DISPLAY_YPOS, dx, PADDLE_DISPLAY_HEIGHT);
		displayPartial(p->x_left, PADDLE_DISPLAY_YPOS, 2*dx + PADDLE_WIDTH, PADDLE_DISPLAY_HEIGHT);
	}else if(dir < 24){
		p->x_left += dx;
		if(p->x_left > p->x_leftbound) p->x_left = p->x_leftbound;
		p->x_right = p->x_left + PADDLE_WIDTH;
		clearRect(p->x_left - dx, PADDLE_DISPLAY_YPOS, dx, PADDLE_DISPLAY_HEIGHT);
		fillRect(p->x_right - dx, PADDLE_DISPLAY_YPOS, dx, PADDLE_DISPLAY_HEIGHT);
		displayPartial(p->x_left - dx, PADDLE_DISPLAY_YPOS, 2*dx + PADDLE_WIDTH, PADDLE_DISPLAY_HEIGHT);
	}
}

void reset_ball(){
	theBall.r = BALL_RADIUS;
	theBall.x = (SCREEN_WIDTH/2) << FP_SHIFT; //top = y + r
	theBall.y = (PADDLE_YPOS - BALL_RADIUS - 1) << FP_SHIFT;
	theBall.LUT_index = 2;
	theBall.dx = bounce_lut[2][0];
	theBall.dy = bounce_lut[2][1];
	theBall.dir_x = 1;
	theBall.dir_y = -1;
	theBall.num_pings = 0;
}

int moveBall(Ball* b, Paddle *p){
	// 1. Erase current ball
	int px = b->x >> FP_SHIFT;
	int py = b->y >> FP_SHIFT;
	int r = b->r;
	eraseCircle(px, py, r);
	displayPartial(px - r, py - r, 2*r+1, 2*r+1);
	// 2. Move the ball
	b->x += b->dx;
	b->y += b->dy;
    // 3. Left/Right wall collisions
    if (px - r <= 0) {
        b->x = (r + 1) << FP_SHIFT; // Snap out of wall
        b->dx *= -1;
    } else if (px + r >= SCREEN_WIDTH - 1) {
        b->x = (SCREEN_WIDTH - r - 2) << FP_SHIFT;
        b->dx *= -1;
    }
    // 4. Ceiling Collision
    if (py - r <= 0) {
        b->y = (r + 1) << FP_SHIFT;
        b->dy *= -1;
    }
    // 5. Floor Collision (Game Over)
    if (py + r >= SCREEN_HEIGHT) {
    	gameOver(b);
        return 0; // Trigger Game Over
    }
    // 6. Paddle Collision. Trajectory changes based on paddle hit zone.
    if (b->dy > 0 && py + r >= PADDLE_YPOS) {
        if (px >= p->x_left && px <= p->x_right) {
            // Snap to top of paddle
            b->y = (PADDLE_YPOS - r - 1) << FP_SHIFT;
            // Increase score
            b->num_pings++;
            // Get zone paddle hits paddle. Update trajectory based on LUT
			int paddle_zone = (px - p->x_left)*11 / PADDLE_WIDTH;
			if(paddle_zone > 10) paddle_zone = 10;
			b->LUT_index = paddle_zone;
            b->dx = bounce_lut[b->LUT_index][0];
            b->dy = bounce_lut[b->LUT_index][1];
            // Increase difficulty
            int speed_boost = 10 * (b->num_pings / 5);
            if (speed_boost > 150) speed_boost = 150; // Cap the boost
            b->dy -= speed_boost;
        }
    }
    // 7. Draw at new pixel position
    px = b->x >> FP_SHIFT;
    py = b->y >> FP_SHIFT;
    drawCircle(px, py, r);
    displayPartial(px - r, py - r, 2*r+1, 2*r+1);
    return 1;
}
