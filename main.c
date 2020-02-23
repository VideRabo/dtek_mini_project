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


int screen = 0;
/*
0 - lobby
1 - leaderboard
2 - game


/* All useful initializationcalls */
void init()
{
return;


}


/* Only display top 3 scores (globally) due to lack of room on display.
   Might be changed */
void leaderboard()
{
    while(1)
    {
	display_string(0, "");
	display_string(1, "");
	display_string(2, "");
	display_string(3, "");
        display_string(0, "Top 3  exit(3)");

	/* IN PROGRESS */
	int i = 0;
	for(i = 0; i < 3; i++)
	{
	//display highscores


	}

	//display highscore
	display_update(0);
        //button 3 to exit to lobby (fancy animation perhaps)
        if(getbtns() & 1)
        {
            screen = 0;
            break;
        }
    }

}

void lobby()
{
    while(1)
    {
        //text, animation
	display_string(0, "");
	display_string(1, "");
	display_string(2, "");
	display_string(0, "LOBBY");
	display_string(1, "(1) game");
	display_string(2, "(2) leaderboard");
        display_update(0);

        /* Game */
      	/*if(getbtns() & 1)
        {
            screen = 2;
            break;
        }*/

        //button 2 to enter leaderboard/highscore list
        /* Leaderboard */
        if(getbtns() & 2)
        {
            screen = 1;
            break;
        }
    }
}


main_loop()
{
    init();
    while(1)
    {
        /* Screen will be initialized to 0. First iteration --> lobby */
        if(screen == 0)
        {
            quicksleep(1000000);
            lobby();
        }

        if(screen == 1)
        {
            quicksleep(1000000);
            leaderboard();
        }

        /* Game */
        if(screen == 2)
        {
	   //gametick etc. If gametick returns 1, we're done
        }
    }

		/* display score. Press 1 to continue to lobby */
        /* In progress */
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
    
    /* Testing menues etc */
    display_string(0, "");
    display_string(1, "");
    display_string(2, "");
    display_string(3, "");
    clearScreen();
    display_update(0);
    display_update(1);
    main_loop();
    
    return 0;
}
