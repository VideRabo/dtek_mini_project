#include <stdio.h>

#include "config.h"
#include "maps.h"
#include "gamelogic.h"
#include "mapmanager.h"

GameObject game_objects_cg1[MAX_NUM_OBJECT]; // game objects collision group 1
int cg1_count = 0; // number of objects in cg1
GameObject game_objects_cg2[MAX_NUM_OBJECT]; // game objects collision group 2
int cg2_count = 0; // number of objects in cg2

// print info about a collision group to console
void print_cg(char name[], GameObject collision_group[], int cg_count) {
    printf("collision group %s [count %d]: \n", name, cg_count);
    for (int i = 0; i < cg_count; i++)
    {
        printf("    type: %d, sub: %d, x: %f, y: %f\n", collision_group[i].type, collision_group[i].subtype, collision_group[i].x, collision_group[i].y);
    }
    printf("\n");
}

void add_ball() {
    GameObject ball = create_game_object(Ball, 1, 0, 0, 10, 10);
    add_game_object(ball, game_objects_cg1, &cg1_count);
}

void game_tick() {
    // update inputs
    // update positions
    update_positions(game_objects_cg1, cg1_count); 
    // check for collisions
    // render
}

int main(void) {
    printf("Hello, World!\n");
    register_map(level_1, "level one");
    int num_maps = get_num_maps();
    char ** map_names = get_map_names();

    printf("n: %d\n", num_maps);

    for (int i = 0; i < num_maps; i++)
    {
        printf("%d: %s\n", i, map_names[i]);
    }
    
    free(map_names); 
    load_registered_map(0, game_objects_cg1, &cg1_count);
    print_cg("cg1", game_objects_cg1, cg1_count);
    add_ball();

    for (int i = 0; i < 10; i++)
    {
        game_tick();
        print_cg("cg1", game_objects_cg1, cg1_count);
    }
    
    return 0;
}