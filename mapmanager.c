#include "mapmanager.h"

static int map_count = 0; // number of maps
static Map registered_maps[MAX_NUM_MAPS];

// register a map with its name and settings - to be loaded with load_registered_map()
void register_map(char map[GRID_ROWS][GRID_COLUMNS], short upgrade_types[], GameSettings map_settings, char *name) {
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

        for (int i = 0; i < MAX_NUM_UPGRADES; i++)
        {
            registered_maps[map_count].upgrade_types[i] = upgrade_types[i];
        }
        

        copy_settings(&registered_maps[map_count].settings, &map_settings);
    
        map_count++;
    }
}

int get_num_maps() {
    return map_count;
}

void cpy_map_names(char * names_dest[MAP_NAME_LEN]) {
    for (int i = 0; i < map_count; i++)
    {
        names_dest[i] = registered_maps[i].name;
    }
    
}

#include "mipslab.h"
#include "display.h"

static void load_map(char map[GRID_ROWS][GRID_COLUMNS], short upgrade_types[], GameObject collision_group[], int *cg_count) {
    int upgrage_index = 0;
    game_status.num_bricks = 0;
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLUMNS; j++)
        {
            //printf("map[%d][%d]: %c\n", i, j, map[i][j]);
            switch (map[i][j])
            {
            case '1':
            case '2':
            case '3':
            {
                GameObject brick = create_game_object(Brick, map[i][j] - 48, BRICK_WIDTH*j, (BRICK_HEIGHT + 1)*i, 0, 0);
                if (map[i][j] - 48 == 2) brick.type_data = upgrade_types[upgrage_index++]; // add specific upgrade type
                if (map[i][j] - 48 != 3) game_status.num_bricks++; // if not unbreakable
                add_game_object(brick, collision_group, cg_count);
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
        load_map(registered_maps[map_index].layout, registered_maps[map_index].upgrade_types, collision_group, cg_count);
        reset_settings();
        update_settings(registered_maps[map_index].settings, 1); // TODO change to 1 ? meaning all settings need to be entered for each map
    }
}

/*
void d_print_map_names() {
    char map_names[MAX_NUM_MAPS][50];
    int num_maps = get_num_maps();
    get_map_names(map_names);
    #if TESTENV == 1
    printf("There are %d registered maps:\n", num_maps);

    for (int i = 0; i < num_maps; i++)
    {
        printf("%d: %s\n", i, map_names[i]);
    }
    
    free(map_names); 
    #endif
}*/