#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "config.h"
#include "gamelogic.h"
#include "settings.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#include "missing_funcs.h"

// STRUCTURES

// container for maps
typedef struct {
    char name[MAP_NAME_LEN];
    char layout[GRID_ROWS][GRID_COLUMNS];
    short upgrade_types[MAX_NUM_UPGRADES];
    GameSettings settings;
} Map;


// PUBLIC FUNCTIONS

// register a map with its name and settings - to be loaded with load_registered_map()
void register_map(char map[GRID_ROWS][GRID_COLUMNS], short upgrade_types[], GameSettings map_settings, char *name);

// get a string array containing names of all registered maps. must be freed
//void get_map_names(char * out[50]);
void cpy_map_names(char * names_dest[MAP_NAME_LEN]);


// get number of registered maps
int get_num_maps();

// create all GameObjects for map and save to collision_group as well as load settings
void load_registered_map(int map_index, GameObject collision_group[], int *cg_count);

// debug. print map names to console
//void d_print_map_names();

#endif