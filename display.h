#ifndef DISPLAY_H
#define DISPLAY_H

#include "gamelogic.h"

void display_clear();
void render(GameObject collision_group[], int cg_count);
void render_menu(char * title, char * option_names[20], void (*option_func[]) (int),  int num_options);

void new_game_animation(char * name);
void lost_ball_animation();
void death_animation();
void cleared_level_animation();
#endif