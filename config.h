#ifndef CONFIG_H
#define CONFIG_H

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

// Physics engine
#define AIR_RESISTANCE 0.5 // ratio of speed that will be subtracted
#define AR_MIN_VELOCITY 2.0 // air resistance min velocity before it has an effect
#define GRAVITY 0

#endif
