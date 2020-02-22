#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "config.h"
#include <stdio.h>
#include <stdlib.h>

enum object_type {Brick = 1, Upgrade = 2, Ball = 3, Pad = 4};

// TODO add collision function pointer
typedef struct game_object {
    short type; // 1: brick, 2: upgrade, 3: ball, 4: pad
    short subtype;

    // size
    short height;
    short width;

    // coordinates
    float x;
    float y;

    // coordinates previous tick, used for collision detection
    float last_x;
    float last_y;

    // speed, is added to coordinates
    float speed_x;
    float speed_y;
    
    short update_position; // if 0, position will not be updated

    // collisions
    short collide; // collision will only be detected if both objects have collide enabled
    void (*collision_function)(struct game_object *, struct game_object *, short); // parameters: this object, other object, collision direction 

    short hitpoints; // -1 if unbreakable

    // physics
    unsigned short mass;
    short gravity; 
} GameObject;

GameObject create_game_object(short type, short subtype, short x, short y, short speed_x, short speed_y);

void add_game_object(GameObject object, GameObject collision_group[], int *cg_count);

void update_positions(GameObject collision_group[], int cg_count);

void check_for_collisions(GameObject cg1[], int cg1_count, GameObject cg2[], int cg2_count);

void collide_ball(GameObject * this, GameObject * other_obj, short direction);


#endif