#ifndef MAPS_H
#define MAPS_H

#include "config.h"
#include "settings.h"
#include "mapmanager.h"

// How to use:
// create 2D char array (GRID_ROWS x GRID_COLUMNS) containing Brick layout for map. 
// The digit corresponds to a Brick subtype. Invalid subtypes will be ignored, just like any other character
// create GameSettings struct with any non-default settings for map.
// register the map, settings, and its name using register_map().
// to activate it, use load_registered_map()


static char level_1[GRID_ROWS][GRID_COLUMNS] = {
    "##############",
    "##11##11##11##",
    "##11##11##11##",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",  
};

static char level_2[GRID_ROWS][GRID_COLUMNS] = {
    "11##########11",
    "12##########21",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",  
};

static short upgrade_types_2[MAX_NUM_UPGRADES] = {2, 2};

static char level_3[GRID_ROWS][GRID_COLUMNS] = {
    "11####33####11",
    "11####33####11",
    "######33######",
    "######33######",
    "#333######333#",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",  
};


static char level_4[GRID_ROWS][GRID_COLUMNS] = {
    "##############",
    "##############",
    "###3######3###",
    "###32122123###",
    "11131111113111",
    "###33333333###",
    "##############",
    "##############",
    "##############",
    "##############",  
};

static short upgrade_types_4[MAX_NUM_UPGRADES] = {4, 4, 4, 4};



static char level_5[GRID_ROWS][GRID_COLUMNS] = {
    "22322111122322",
    "##311####113##",
    "##3########3##",
    "##3##3333##3##",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",
    "##############",  
};

static short upgrade_types_5[MAX_NUM_UPGRADES] = {5, 3, 5, 3, 3, 5, 3, 5};


void init_maps() {
    short upgrade_types_d[MAX_NUM_UPGRADES]; 
    GameSettings settings = (GameSettings) {};

    // LEVEL 1: lotta bricks
    register_map(level_1, upgrade_types_d, settings, "simple");

    // LEVEL 2: pretty far
    settings = (GameSettings) {
        .ball_speed = 2.5,
        .ball_size = 3,
        .gravity_acceleration = 0.1
    };

    register_map(level_2, upgrade_types_2, settings, "pretty far");

    // LEVEL 3: now what?
    settings = (GameSettings) {
        .ball_speed = 2.5,
        .gravity_acceleration = 0.1,
        .pad_size = 20
    };
    register_map(level_3, upgrade_types_d, settings, "now what?");

     // LEVEL 4: a lot of balls
    settings = (GameSettings) {
        .pad_size = 20
    };
    register_map(level_4, upgrade_types_4, settings, "a lot of balls");

    // LEVEL 5: pew pew
    settings = (GameSettings) {
        .ball_speed = 3,
        .ar_minimum_velocity = 1,
        .air_resistance = 0.1
    };
    register_map(level_5, upgrade_types_5, settings, "pew pew");
}

#endif