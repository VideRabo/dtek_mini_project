#ifndef CONFIG_H
#define CONFIG_H

// Environment
#define TESTENV 0

// Hardware specifics
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

// Grid
#define GRID_COLUMNS 10
#define GRID_ROWS 10
#define BRICK_WIDTH DISPLAY_WIDTH / GRID_COLUMNS
#define BRICK_HEIGHT DISPLAY_HEIGHT / GRID_ROWS


// Maps
#define MAX_NUM_MAPS 3

// Game logic
#define MAX_NUM_OBJECT 50 // per collision group

// Game Settings
#define PAD_SIZE 10
#define BALL_SIZE 2
#define BALL_SPEED 1.0
#define AIR_RESISTANCE 0.2 // ratio of speed that will be subtracted
#define AR_MIN_VELOCITY 2.0 // air resistance min velocity before it has an effect
#define GRAVITY_ACCELERATION 0.0

#endif
