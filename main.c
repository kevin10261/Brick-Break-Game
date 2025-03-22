// Include FreeRTOS Libraries
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Include xilinx Libraries
#include "xparameters.h"
#include "xgpio.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xil_cache.h"

// Other miscellaneous libraries
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "pmodkypd.h"
#include "sleep.h"
#include "PmodOLED.h"
#include "OLEDControllerCustom.h"


#define BTN_DEVICE_ID  XPAR_INPUTS_DEVICE_ID
#define BTN_CHANNEL    1
#define PADDLE_WIDTH  15
#define PADDLE_HEIGHT 2
#define PADDLE_Y      28
#define BALL_SIZE     2
#define SCREEN_WIDTH  32
#define SCREEN_HEIGHT 128
#define BRICK_ROWS    2
#define BRICK_COLS    8
#define BRICK_WIDTH   14
#define BRICK_HEIGHT  3
#define KYPD_DEVICE_ID XPAR_KEYPAD_DEVICE_ID
#define KYPD_BASE_ADDR XPAR_KEYPAD_BASEADDR


#define FRAME_DELAY 30000

// keypad key table
#define DEFAULT_KEYTABLE 	"0FED789C456B123A"

// Declaring the devices
XGpio       btnInst;
PmodOLED    oledDevice;
PmodKYPD 	KYPDInst;

// Game variables
u8 paddle_y = 24;
u8 paddle_x = (SCREEN_HEIGHT - PADDLE_WIDTH) / 2;
u8 ball_x = 64, ball_y = 16;
int ball_dx = 1, ball_dy = -1;
int score = 0, lives = 3;
u8 bricks[BRICK_ROWS][BRICK_COLS];


// Function prototypes
void initializeScreen();
void drawGame();
void updateBall();
void movePaddle(u8 direction);



static void oledTask( void *pvParameters );

// To change between PmodOLED and OnBoardOLED is to change Orientation
const u8 orientation = 0x0; // Set up for Normal PmodOLED(false) vs normal
                            // Onboard OLED(true)
const u8 invert = 0x0; // true = whitebackground/black letters
                       // false = black background /white letters
u8 keypad_val = 'x';


int main()
{
	int status = 0;
	// Initialize Devices
	initializeScreen();

	// Buttons
	status = XGpio_Initialize(&btnInst, BTN_DEVICE_ID);
	if(status != XST_SUCCESS){
		xil_printf("GPIO Initialization for SSD failed.\r\n");
		return XST_FAILURE;
	}
	XGpio_SetDataDirection (&btnInst, BTN_CHANNEL, 0x0f);


	xil_printf("Initialization Complete, System Ready!\n");


	xTaskCreate( oledTask					/* The function that implements the task. */
			   , "screen task"				/* Text name for the task, provided to assist debugging only. */
			   , configMINIMAL_STACK_SIZE	/* The stack allocated to the task. */
			   , NULL						/* The task parameter is not used, so set to NULL. */
			   , tskIDLE_PRIORITY			/* The task runs at the idle priority. */
			   , NULL
			   );

	vTaskStartScheduler();


   while(1);

   return 0;
}

void initializeScreen()
{
   OLED_Begin(&oledDevice, XPAR_PMODOLED_0_AXI_LITE_GPIO_BASEADDR,
         XPAR_PMODOLED_0_AXI_LITE_SPI_BASEADDR, orientation, invert);
   memset(bricks,1,sizeof(bricks));
}

void InitializeKeypad()
{
   KYPD_begin(&KYPDInst, KYPD_BASE_ADDR);
   KYPD_loadKeyTable(&KYPDInst, (u8*) DEFAULT_KEYTABLE);
}


void drawCrossHair(u8 xco, u8 yco)
{
	OLED_MoveTo(&oledDevice, xco, 0);
	OLED_DrawLineTo(&oledDevice, xco, OledRowMax - 1);
	OLED_MoveTo(&oledDevice, 0, yco);
	OLED_DrawLineTo(&oledDevice, OledColMax - 1, yco);
}

void drawGame() {
    OLED_ClearBuffer(&oledDevice);

    // Draw paddle
    OLED_MoveTo(&oledDevice, paddle_x, PADDLE_Y);
    OLED_DrawLineTo(&oledDevice, paddle_x + PADDLE_WIDTH, PADDLE_Y);

    // Draw ball
    OLED_MoveTo(&oledDevice, ball_x, ball_y);
    OLED_DrawLineTo(&oledDevice, ball_x + BALL_SIZE, ball_y + BALL_SIZE);

    // Draw bricks
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            if (bricks[row][col]) {
                OLED_MoveTo(&oledDevice, col * BRICK_WIDTH, row * BRICK_HEIGHT);
                OLED_RectangleTo(&oledDevice, (col + 1) * BRICK_WIDTH, (row + 1) * BRICK_HEIGHT);
            }
        }
    }

    // Draw score & lives
    OLED_SetCursor(&oledDevice, 0, 3);
    char buffer[20];
//    sprintf(buffer, "Score: %d Lives: %d", score, lives);
    OLED_PutString(&oledDevice, buffer);

    OLED_Update(&oledDevice);
}


// Update ball movement & collision
void updateBall() {
    ball_x += ball_dx;
    ball_y += ball_dy;

    // Wall collision
    if (ball_x <= 0 || ball_x >= SCREEN_HEIGHT - BALL_SIZE) ball_dx *= -1;
    if (ball_y <= 0) ball_dy *= -1;

    // Paddle collision: Check if the ball is within the paddle's y-range and x-range
    if (ball_y + BALL_SIZE >= PADDLE_Y && ball_y <= PADDLE_Y + PADDLE_HEIGHT) {
        if (ball_x + BALL_SIZE >= paddle_x && ball_x <= paddle_x + PADDLE_WIDTH) {
            // Reverse the ball's vertical direction on collision
            ball_dy *= -1;


            int paddle_center = paddle_x + PADDLE_WIDTH / 2;
            int ball_center = ball_x + BALL_SIZE / 2;
            int distance_from_center = ball_center - paddle_center;

            // Apply a factor to control the angle (tuning factor)
            float angle_factor = 0.25; // Reduced angle factor for even slower horizontal movement

            // Modify ball's horizontal direction based on where it hits the paddle
            ball_dx += distance_from_center * angle_factor;

            // Limit the speed to prevent the ball from moving too fast horizontally
            if (ball_dx > 1) ball_dx = 1; // Max horizontal speed to the right
            if (ball_dx < -1) ball_dx = -1; // Max horizontal speed to the left
        }
    }

    // Brick collision
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            int bx = col * BRICK_WIDTH;
            int by = row * BRICK_HEIGHT;
            if (bricks[row][col] && ball_x + BALL_SIZE > bx && ball_x < bx + BRICK_WIDTH &&
                ball_y + BALL_SIZE > by && ball_y < by + BRICK_HEIGHT) {
                bricks[row][col] = 0;
                score += 10;
                ball_dy *= -1;  // Reverse ball direction after hitting a brick
            }
        }
    }


    if (ball_y >= SCREEN_HEIGHT) {
        lives--;

        ball_x = SCREEN_WIDTH / 2;
        ball_y = SCREEN_HEIGHT / 2;

        // Randomized direction towards bricks with lower speed
        ball_dx = (rand() % 2 == 0 ? 1 : -1) * (rand() % 2 + 1);
        ball_dy = -1;  // Always move upwards when respawning
    }
}





void movePaddle(u8 direction) {
    if (direction == 1 && paddle_x > 0) paddle_x -= 2; // Move left
    if (direction == 2 && paddle_x < SCREEN_HEIGHT - PADDLE_WIDTH) paddle_x += 2; // Move right
}

static void oledTask( void *pvParameters )
{
    u8 buttonVal;
    while (lives > 0) {
        buttonVal = XGpio_DiscreteRead(&btnInst, BTN_CHANNEL);
        if (buttonVal == 1) movePaddle(1);
        if (buttonVal == 2) movePaddle(2);

        updateBall();
        drawGame();

        usleep(FRAME_DELAY);
    }

    // Game Over
    OLED_ClearBuffer(&oledDevice);
    OLED_SetCursor(&oledDevice, 2, 1);
    OLED_PutString(&oledDevice, "Game Over");
    OLED_Update(&oledDevice);
}
