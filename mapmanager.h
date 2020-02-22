#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "config.h"
#include "gamelogic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char name[50];
    char layout[GRID_ROWS][GRID_COLUMNS];
} Map;

void register_map(char map[GRID_ROWS][GRID_COLUMNS], char name[50]);

char** get_map_names();

int get_num_maps();

void load_registered_map(int map_index, GameObject collision_group[], int *cg_count);

#endif