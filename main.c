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
    printf("%s [count %d]: \n", name, cg_count);
    for (int i = 0; i < cg_count; i++)
    {
        printf("    type: %d, sub: %d, x: %.1f (%.1f), y: %.1f (%.1f) - [W x H]: %dx%d\n", collision_group[i].type, collision_group[i].subtype, collision_group[i].x, 
        collision_group[i].speed_x, collision_group[i].y, collision_group[i].speed_y, collision_group[i].width, collision_group[i].height);
    }
}

void add_ball() {
    GameObject ball = create_game_object(Ball, 1, 16, 10, 1, 0);
    add_game_object(ball, game_objects_cg2, &cg2_count);
}

void game_tick() {
    // check for collisions
    check_for_collisions(game_objects_cg1, cg1_count, game_objects_cg2, cg2_count);
    // update inputs
    // update positions
    update_positions(game_objects_cg1, cg1_count);
    update_positions(game_objects_cg2, cg2_count);
    update_buffs(); 
    
    // render
}

int main(void) {
    reset_settings();

    // maps
    register_map(level_1, level_1_settings, "level one");
    d_print_map_names();
    load_registered_map(0, game_objects_cg1, &cg1_count);
    
    // settings and buffs
    // add_buffs((GameSettings) {.ball_speed = 10, .ball_size = 10}, (SettingsDuration) {.ball_speed = 2, .ball_size = 1});
    //d_print_settings();

    add_ball();
    //add_buffs((GameSettings){.gravity_acceleration = 0.0}, (SettingsDuration) {.gravity_acceleration = 4});

    for (int i = 0; i < 6; i++)
    {
        print_cg("cg1",game_objects_cg1, cg1_count);
        print_cg("cg2",game_objects_cg2, cg2_count);
        game_tick();
    }
    

    return 0;
}