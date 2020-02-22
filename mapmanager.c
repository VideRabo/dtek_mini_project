#include "mapmanager.h"

static int map_count = 0; // number of maps
static Map registered_maps[MAX_NUM_MAPS];

void register_map(char map[GRID_ROWS][GRID_COLUMNS], char *name) {
    if (map_count <= MAX_NUM_MAPS) {
        registered_maps[map_count] = (Map) {};
        strcpy(registered_maps[map_count].name, name);

        // copy layout of map
        for (int i = 0; i < GRID_ROWS; i++)
        {
            for (int j = 0; j < GRID_COLUMNS; j++)
            {
                registered_maps[map_count].layout[i][j] = map[i][j];
            }
        }
    
        map_count++;
    }
}

// note return value must be freed
char** get_map_names() {
    char **map_names = (char **) malloc(map_count*50*sizeof(char));
    for (int i = 0; i < map_count; i++)
    {
        map_names[i] = registered_maps[i].name;
    }
    return map_names;
}

int get_num_maps() {
    return map_count;
}

static void load_map(char map[GRID_ROWS][GRID_COLUMNS], GameObject collision_group[], int *cg_count) {
    for (int i = 0; i < GRID_COLUMNS; i++)
    {
        for (int j = 0; j < GRID_ROWS; j++)
        {
            //printf("map[%d][%d]: %c\n", i, j, map[i][j]);
            switch (map[i][j])
            {
            case '1':
            {
                GameObject object = create_game_object(Brick, map[i][j] - 48, BRICK_WIDTH*i, BRICK_HEIGHT*j, 0, 0);
                //printf("created object: %d %d\n", object.type, object.subtype);
                add_game_object(object, collision_group, cg_count);
                break;
            } 
            default:
                // invalid subtype, do nothing
                break;
            }
        } 
    }
}

void load_registered_map(int map_index, GameObject collision_group[], int *cg_count) {
    if (map_index >= 0 && map_index < map_count) {
        load_map(registered_maps[map_index].layout, collision_group, cg_count);
    }
}