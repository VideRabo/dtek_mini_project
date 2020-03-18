#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "config.h"
#include "settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
// TYPES

enum object_type {Brick = 1, Upgrade = 2, Ball = 3, Pad = 4, Barrier = 5};

// TODO add collision function pointer
typedef struct game_object {
    short type; // 1: brick, 2: upgrade, 3: ball, 4: pad
    short subtype; // under categories e.g. basic brick, unbreakable brick
    short type_data; // used for e.g. what type of upgrade to drop

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
    
    short update_position; // if false, position will not be updated

    // collisions
    short collide; // collision will only be detected if both objects have collide enabled
    void (*collision_function)(struct game_object *, struct game_object *, short); // handles the collision for this object. parameters: this object, other object, collision direction 
    
    // interaction
    short penetrable; // if true, other objects will pass trough rather than bounce on this object when colliding
    short damage; // nr hitpoints to subtract from other object on collision
    short hitpoints; // -1 if unbreakable

    // physics
    short gravity; // if true, object will be affected by gravity
} GameObject;


// PUBLIC VARIABLES

extern GameObject game_objects_cg1[MAX_NUM_OBJECT]; // game objects collision group 1
extern int cg1_count; // number of objects in cg1
extern GameObject game_objects_cg2[MAX_NUM_OBJECT]; // game objects collision group 2
extern int cg2_count; // number of objects in cg2


// contains info for active game
struct {
    short lives;
    int points;
    int num_balls;
    int num_bricks;
    short stage; // 0: ready to launch ball. 1: in game
} game_status;

GameObject * player_pad;

// PUBLIC FUNCTIONS

// utility function to easily create presets of GameObjects with different type and subtype
GameObject create_game_object(short type, short subtype, short x, short y, short speed_x, short speed_y);

// add a GameObject to a collision group
GameObject * add_game_object(GameObject object, GameObject collision_group[], int *cg_count);

// update position and speed for for all GameObjects in a given collision group
void update_positions(GameObject collision_group[], int cg_count);

// check if object collides and if thats the case, call its collision function
void check_for_collisions(GameObject cg1[], int cg1_count, GameObject cg2[], int cg2_count);

void remove_game_object(GameObject * object);

// COLLISION FUNCTIONS

void collide_ball(GameObject * this, GameObject * other_obj, short direction);

void collide_brick(GameObject * this, GameObject * other_obj, short direction);

void collide_upgrade(GameObject * this, GameObject * other_obj, short direction);

#endif