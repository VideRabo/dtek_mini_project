#ifndef CONFIG_H
#define CONFIG_H

// Environment
#define TESTENV 0

// Hardware specifics
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

// Timer
#define TMR2PERIOD ((80000000 / 256) / 26) // period with 1 : 256 prescalor and 80 MHz clock

// Grid
#define GRID_COLUMNS 14
#define GRID_ROWS 10
#define BRICK_WIDTH DISPLAY_WIDTH / GRID_COLUMNS
#define BRICK_HEIGHT DISPLAY_HEIGHT / GRID_ROWS


// Maps
#define MAX_NUM_MAPS 5 // maximum number of maps that can be registered by the map manager
#define MAP_NAME_LEN 20 
#define MAX_NUM_UPGRADES 20

// Game logic
#define MAX_NUM_OBJECT 50 // per collision group

// Non changable game settings
#define START_LIVES 3 // Number of lives on start of level
#define SESSION_START_LIVES 5
#define PAD_NO_CURVE 6 // area on pad that does not curve ball
#define PAD_MOVE_DISTANCE 3 // nr pixels a player can move the pad each tick
#define UPGRADE_SIZE 4 // pixels width and height of upgrades
#define UPGRADE_SPEED 1 // speed at which upgrades fall

// Game Settings
#define PAD_SIZE 15
#define BALL_SIZE 2
#define BALL_SPEED 2.0
#define AIR_RESISTANCE 0.2 // ratio of speed that will be subtracted
#define AR_MIN_VELOCITY 2.0 // air resistance min velocity before it has an effect
#define GRAVITY_ACCELERATION 0.0

#endif
