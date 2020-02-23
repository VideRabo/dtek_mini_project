#ifndef MAPS_H
#define MAPS_H

#include "config.h"
#include "settings.h"

// How to use:
// create 2D char array (GRID_ROWS x GRID_COLUMNS) containing Brick layout for map. 
// The digit corresponds to a Brick subtype. Invalid subtypes will be ignored, just like any other character
// create GameSettings struct with any non-default settings for map.
// register the map, settings, and its name using register_map().
// to activate it, use load_registered_map()

char level_1[GRID_ROWS][GRID_COLUMNS] = {
    "##########",
    "#1########",
    "##########",
    "##########",
    "##########",
    "##########",
    "##########",
    "##########",
    "##########",
    "##########",
    
};

GameSettings level_1_settings = (GameSettings) {
    .ball_speed = 2
};

#endif