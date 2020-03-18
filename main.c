#include <stdio.h>

#include "config.h"
#include "maps.h"
#include "gamelogic.h"
#include "mapmanager.h"
#include <stdint.h>

#include "input.h"
#include "mipslab.h"
#include "display.h"

GameObject game_objects_cg1[MAX_NUM_OBJECT]; // game objects collision group 1
int cg1_count = 0; // number of objects in cg1
GameObject game_objects_cg2[MAX_NUM_OBJECT]; // game objects collision group 2
int cg2_count = 0; // number of objects in cg2

// chooses which screen to display. 0: main menu, 1: level select, 2: game
int screen = 0;

struct {
    short active; // is game a session
    short map_index; // current level in session
} session_status;

// menu variables

static char * main_menu_opt_names[20]; 
static void (*main_menu_opt_funcs[2]) (int) = {};
static int num_maps;
static char * level_menu_opt_names[MAP_NAME_LEN]; 
static void (*level_menu_opt_funcs[MAX_NUM_MAPS+1]) (int) = {};

// add player pad
void add_pad() {
    GameObject pad = create_game_object(Pad, 1, (DISPLAY_WIDTH - PAD_SIZE) / 2, DISPLAY_HEIGHT - 2, 0, 0);
    player_pad = add_game_object(pad, game_objects_cg1, &cg1_count);
}

// reset game status and add pad
void init_game() {
    game_status.lives = session_status.active ? SESSION_START_LIVES : START_LIVES;
    game_status.points = 0;
    game_status.num_balls = 0;
    game_status.stage = 0;

    add_pad();
}

// remove all GameObjects
void reset_game() {
    cg1_count = 0;
    cg2_count = 0;
}


// menu select functions

// go to level select screen
void opt_sel_level(int n) {
    screen = 1;
}

// start a new session
void opt_new_session(int n) {
    screen = 2;
    session_status.active = 1;
    session_status.map_index = 0;
    reset_game();
    load_registered_map(0, game_objects_cg1, &cg1_count);
    init_game();
    new_game_animation(level_menu_opt_names[0]);
}

// go to main screen
void opt_sel_main(int n) {
    screen = 0;
}

// start a level
void opt_init_level(int n) {
    screen = 2;
    reset_game();
    load_registered_map(n, game_objects_cg1, &cg1_count);
    init_game();
    new_game_animation(level_menu_opt_names[n]);
}

// register all menu funcitons - need to run before using menus
void init_menus() {
    // main menu
    main_menu_opt_names[0] = "select a level";
    main_menu_opt_funcs[0] = &opt_sel_level;
    main_menu_opt_names[1] = "start new game";
    main_menu_opt_funcs[1] = &opt_new_session;
    
    // level menu
    num_maps = get_num_maps();
    cpy_map_names(level_menu_opt_names);
    for (int i = 0; i < num_maps; i++)
    {
        level_menu_opt_funcs[i] = &opt_init_level;   
    }
    
    level_menu_opt_names[num_maps] = "[back]";
    level_menu_opt_funcs[num_maps] = &opt_sel_main;
}

// launch ball from player pad
void launch_ball() {
    if (player_pad) {
        float pad_mid = player_pad->x + player_pad->width/2; 
        GameObject ball = create_game_object(Ball, 1, pad_mid, player_pad->y - game_settings.ball_size, 0, -game_settings.ball_speed);
        add_game_object(ball, game_objects_cg2, &cg2_count);
        game_status.num_balls++;
    }
}

// update player pad position
void move_player_pad() {
    if (input_status.btn2 && player_pad && player_pad->x > 0) player_pad->x -= PAD_MOVE_DISTANCE; 
    if (input_status.btn1 && player_pad && player_pad->x + player_pad->width < DISPLAY_WIDTH) player_pad->x += PAD_MOVE_DISTANCE; 
}

// load next level in a session
void session_next_level() {
    if (get_num_maps() - 1 > session_status.map_index) { // if there are maps left
        // reset game but keep most game status properties
        reset_game();
        load_registered_map(++session_status.map_index, game_objects_cg1, &cg1_count);
        add_pad();
        game_status.stage = 0;
        game_status.num_balls = 0;
        new_game_animation(level_menu_opt_names[session_status.map_index]);
        screen = 2;
    } else { // all levels cleared
        session_status.active = 0;
        screen = 0;
    }
}

// check if there are any balls left, if not remove 1 life and change to pre launch stage
void check_for_death() {
    if (game_status.num_balls < 1) { // no balls left
        game_status.lives--;
        game_status.stage = 0;
        if (game_status.lives < 1) { // no lives left
            session_status.active = 0;
            screen = 1;
            death_animation();
        } else { // still alive
            lost_ball_animation();
        }
    }
}

// check if there are any bricks left
void check_for_victory() {
    if (game_status.num_bricks < 1) {
        cleared_level_animation();
        if (session_status.active) {
            session_next_level();
        } else {
            screen = 1;
        }
    }
}

// runs every iteration in game
void game_tick() {
    move_player_pad(); // always allow player to move pad

    if (game_status.stage == 1) { // in game
        check_for_collisions(game_objects_cg1, cg1_count, game_objects_cg2, cg2_count);
        update_positions(game_objects_cg1, cg1_count);
        update_positions(game_objects_cg2, cg2_count);
        update_buffs(); 
        check_for_death();
        check_for_victory();

    } else if (game_status.stage == 0) { // pre ball launch
        if (input_status.btn3) {
            launch_ball();
            game_status.stage = 1;
        }
    }

    if (input_status.sw1) { // exit level 
        session_status.active = 0;
        screen = 1;
    }
    
    // render
    display_clear();
    render(game_objects_cg1, cg1_count);
    render(game_objects_cg2, cg2_count);
	display_update(1);
}

void main_tick() {
    if (screen == 0) { // main menu
        render_menu("Brick Breaker V1", main_menu_opt_names, main_menu_opt_funcs, 2);
    } else if (screen == 1) { // level select menu
        render_menu("Select a Level", level_menu_opt_names, level_menu_opt_funcs, num_maps + 1);
    } else if (screen == 2) { // in game
        game_tick();
    }
}

int main(void) {
    setup_hardware();	
	display_init();
    
    init_maps(); // register all maps
    init_menus(); 
    reset_settings(); // set default settings (from config.h)

    // main loop
    for(;;) {
        if (timer_ready()) {
            update_inputs(); // updates input_status object
            main_tick();
        }
    }

    return 0;
}
