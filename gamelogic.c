#include "gamelogic.h"

// utility function for easily creating GameObjects
GameObject create_game_object(short type, short subtype, short x, short y, short speed_x, short speed_y) {
    
    GameObject ret = {
            .subtype = subtype,
            .x = x,
            .y = y,
            .last_x = x,
            .last_y = y,
            .speed_x = speed_x,
            .speed_y = speed_y
    };

    switch (type)
    {
    case Brick:
        ret.type = Brick;
        ret.collide = 1;
        ret.collision_function = &collide_brick;
        ret.update_position = 0;
        ret.hitpoints = 1;
        ret.gravity = 0;
        ret.height = BRICK_HEIGHT;
        ret.width = BRICK_WIDTH;
        return ret;
        break;

    case Upgrade:
        ret.type = 0; // invalid type
        return ret;

    case Ball:
        ret.type = Ball; // invalid type
        ret.collide = 1;
        ret.gravity = 1;
        ret.update_position = 1;
        ret.damage = 1;
        ret.collision_function = &collide_ball;
        ret.height = game_settings.ball_size;
        ret.width = game_settings.ball_size;
        return ret;

    case Pad:
        ret.type = 0; // invalid type
        return ret;

    default:
        ret.type = 0; // invalid type
        return ret;
    }
}

// add a copy of a GameObject to a given collision_group
void add_game_object(GameObject object, GameObject collision_group[], int *cg_count) {
    if(*cg_count <= MAX_NUM_OBJECT) {
        collision_group[(*cg_count)++] = object;
    }
}

// update position and speed of a singe GameObject
static void update_position(GameObject * object) {
    object->last_x = object->x;
    object->last_y = object->y;
    
    object->x += object->speed_x;
    object->y += object->speed_y;

    object->speed_x += (float) (fabs(object->speed_x) > game_settings.ar_minimum_velocity) ? (-object->speed_x * game_settings.air_resistance) : 0; 
    object->speed_y += (float) ((fabs(object->speed_y) > game_settings.ar_minimum_velocity) ? (-object->speed_y * game_settings.air_resistance) : 0) + object->gravity*game_settings.gravity_acceleration;
}

// update position for all objects in a collision group
void update_positions(GameObject collision_group[], int cg_count) {
    for (int i = 0; i < cg_count; i++)
    {
        if (collision_group[i].update_position)
        {
            update_position(&collision_group[i]);
        }
    }
}

static short get_collision_direction(GameObject * obj1, GameObject * obj2) {
    
    short collide_left = 0;
    short collide_right = 0;
    short collide_top = 0;
    short collide_bottom = 0;

    {
        int obj1_left = obj1->x;
        int obj1_last_left = obj1->last_x;
        int obj2_right = obj2->x + obj2->width;
        int obj2_last_right = obj2->last_x + obj2->width;

        collide_left = obj1_last_left > obj2_last_right && obj1_left <= obj2_right; 
    }

    {
        int obj1_right = obj1->x + obj1->width;
        int obj1_last_right = obj1->last_x + obj1->width;
        int obj2_left = obj2->x;
        int obj2_last_left = obj2->last_x;

        collide_right = obj1_last_right < obj2_last_left && obj1_right >= obj2_left; 
    }
    
    {
        int obj1_bottom = obj1->y + obj1->height;
        int obj1_last_bottom = obj1->last_y + obj1->height;
        int obj2_top = obj2->y;
        int obj2_last_top = obj2->last_y;

        collide_bottom = obj1_last_bottom < obj2_last_top && obj1_bottom >= obj2_top; 
    }

    {
        int obj1_top = obj1->y;
        int obj1_last_top = obj1->last_y;
        int obj2_bottom = obj2->y + obj2->height;
        int obj2_last_bottom = obj2->last_y + obj2->height;

        collide_top = obj1_last_top > obj2_last_bottom && obj1_top <= obj2_bottom; 
    }

    printf("l: %d, r: %d, t: %d, b: %d\n", collide_left, collide_right, collide_top, collide_bottom);
    // bit 0: left, bit 1: right, bit 2: top, bit 3: bottom;
    return collide_left + (collide_right << 1) + (collide_top << 2) + (collide_bottom << 3);
}

// reverse collision direction
static short rev_col_dir(short col_dir) {
    return ((col_dir & 0x1) << 1) + ((col_dir & 0x2) >> 1) + ((col_dir & 0x4) << 1) + ((col_dir & 0x8) >> 1);
}

static int check_for_collision(GameObject * obj1, GameObject * obj2) {
    int obj1_top = obj1->y;
    int obj1_bottom = obj1->y + obj1->height;
    int obj1_right = obj1->x + obj1->width;
    int obj1_left = obj1->x;
    int obj2_top = obj2->y;
    int obj2_bottom = obj2->y + obj2->height;
    int obj2_right = obj2->x + obj2->width;
    int obj2_left = obj2->x;

    short overlap_x = obj1_right >= obj2_left && obj1_left <= obj2_right;
    short overlap_y = obj1_bottom >= obj2_top && obj1_top <= obj2_bottom;

    return overlap_x && overlap_y; 
}

// check for collisions between two collision groups
void check_for_collisions(GameObject cg1[], int cg1_count, GameObject cg2[], int cg2_count) {
    for (int i = 0; i < cg1_count; i++)
    {
        if (cg1[i].collide) {
            for (int j = 0; j < cg2_count; j++)
            {
                if(cg2[j].collide) {
                    printf("checking\n");
                    if (check_for_collision(&cg1[i], &cg2[j])) {
                        printf("Overlap confirmed\n");
                        short collision_direction = get_collision_direction(&cg1[i], &cg2[j]);
                        if (cg1[i].collision_function) (*(cg1[i].collision_function))(&cg1[i], &cg2[j], collision_direction);
                        if (cg2[j].collision_function) (*(cg2[j].collision_function))(&cg2[j], &cg1[i], rev_col_dir(collision_direction)); 
                    }
                }
            }
        }
    }
}

// change direction of object depending on bounce direction
void bounce(GameObject * object, short direction) {
    if (direction & 0xc) { // top or bottom
        object->speed_y = -object->speed_y; 
    }
    
    if (direction & 0x3) { // left or right
        object->speed_x = -object->speed_x; 
    }
}

// deals damage from dealer to reciever. returns true if reciever dies
int damage(GameObject * reciever, GameObject * dealer) {
    if (reciever->hitpoints > -1) {
        reciever->hitpoints -= dealer->damage;
        if(reciever->hitpoints < 1) return 1;
    }
    return 0;
}

void collide_ball(GameObject * this, GameObject * other_obj, short direction) {
    switch (other_obj->type)
    {
    case Pad:
        printf("DEAL WITH IT");
        break;
    
    default:
        if (!other_obj->penetrable) {
            bounce(this, direction);
        }
        break;
    }
    
    printf("ball collided in direction: %d\n", direction);
}

void collide_brick(GameObject * this, GameObject * other_obj, short direction) {
    switch (other_obj->type)
    {
    case Ball:
        if (damage(this, other_obj)) {
            this->type = 0;
            this->collide = 0;
        }
        break;
    
    default:
        break;
    }
    
    printf("Brick collided in direction: %d\n", direction);
}