#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "config.h"
#include "gamelogic.h"
#include "settings.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// STRUCTURES

// container for maps
typedef struct {
    char name[50];
    char layout[GRID_ROWS][GRID_COLUMNS];
    GameSettings settings;
} Map;


// PUBLIC FUNCTIONS

// register a map with its name and settings - to be loaded with load_registered_map()
void register_map(char map[GRID_ROWS][GRID_COLUMNS], GameSettings map_settings, char *name);

// get a string array containing names of all registered maps. must be freed
char** get_map_names();

// get number of registered maps
int get_num_maps();

// create all GameObjects for map and save to collision_group as well as load settings
void load_registered_map(int map_index, GameObject collision_group[], int *cg_count);

// debug. print map names to console
void d_print_map_names();

#endif