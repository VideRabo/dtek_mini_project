#include "gamelogic.h"

#include <string.h>

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

        //test
        ret.type_data = 4; // upgrade type

        if (subtype == 3) { // unbreakable
            ret.hitpoints = -1;
        }
        return ret;
        break;

    case Upgrade:
        ret.type = Upgrade; // invalid type
        ret.collide = 1;
        ret.collision_function = &collide_upgrade;
        ret.update_position = 1;
        ret.hitpoints = -1;
        ret.gravity = 0;
        ret.height = UPGRADE_SIZE;
        ret.width = UPGRADE_SIZE;
        return ret;

    case Ball:
        ret.type = Ball; // invalid type
        ret.collide = 1;
        ret.gravity = 1;
        ret.update_position = 1;
        ret.damage = 1;
        ret.hitpoints = 1;
        ret.collision_function = &collide_ball;
        ret.height = game_settings.ball_size;
        ret.width = game_settings.ball_size;
        return ret;

    case Pad:
        ret.type = Pad;
        ret.collide = 1;
        ret.gravity = 0;
        ret.update_position = 0;
        ret.damage = 0;
        ret.hitpoints = -1;
        ret.height = 4;
        ret.width = game_settings.pad_size;
        return ret;

    case Barrier:
        ret.type = Barrier;
        ret.collide = 1;
        ret.gravity = 0;
        ret.update_position = 0;

        ret.width = DISPLAY_WIDTH + 20;
        ret.height = DISPLAY_HEIGHT + 20;
        
        if (subtype == 1) {
            ret.damage = 0;
        } else if (subtype == 2) {
            ret.damage = 1;
        }

        return ret;
    default:
        ret.type = 0; // invalid type
        return ret;
    }
}

// add a copy of a GameObject to a given collision_group
GameObject * add_game_object(GameObject object, GameObject collision_group[], int *cg_count) {
    if(*cg_count <= MAX_NUM_OBJECT) {
        collision_group[*cg_count] = object;
        return &collision_group[(*cg_count)++];
    }
    return NULL;
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

// set velocity of GameObject - absolute speed, and angle relative to current direction
void set_velocity(GameObject * object, short speed, int angle) {
    float last_speed_x = object->speed_x + 0.0001;
    float last_speed_y = object->speed_y + 0.0001;
    
    //object->speed_x = speedup * (last_speed_x * cos(angle) - last_speed_y * sin(angle)); 
    //object->speed_y = speedup * (last_speed_x * sin(angle) + last_speed_y * cos(angle)); 

    float mult_term = 1/(sqrt(1 + (last_speed_y*last_speed_y)/(last_speed_x*last_speed_x)));

    object->speed_x = speed * mult_term * (cos(angle) - last_speed_y/last_speed_x * sin(angle)); 
    object->speed_y = speed * mult_term * (sin(angle) + last_speed_y/last_speed_x * cos(angle)); 
}

// change direction of object depending on bounce direction, also sets speed to current ball_speed
void bounce(GameObject * object, short direction) {
    float speed_ratio = game_settings.ball_speed/sqrt(object->speed_x*object->speed_x + object->speed_y*object->speed_y);
    
    if (direction & 0x1) { // left
        object->speed_x = fabs(object->speed_x) * speed_ratio; 
    } else if (direction & 0x2) { // right
        object->speed_x = -fabs(object->speed_x) * speed_ratio; 
    } else if (direction & 0x4) { // top
        object->speed_y = fabs(object->speed_y) * speed_ratio;
    } else if (direction & 0x8) { // bottom
        object->speed_y = -fabs(object->speed_y) * speed_ratio; 
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

void remove_game_object(GameObject * object) {
    object->type = 0;
    object->collide = 0;
    object->update_position = 0;
}

// returns short indicating in which direction obj1 collided with obj2
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

    //printf("l: %d, r: %d, t: %d, b: %d\n", collide_left, collide_right, collide_top, collide_bottom);
    // bit 0: left, bit 1: right, bit 2: top, bit 3: bottom;
    return collide_left + (collide_right << 1) + (collide_top << 2) + (collide_bottom << 3);
}

// reverse collision direction
static short rev_col_dir(short col_dir) {
    return ((col_dir & 0x1) << 1) + ((col_dir & 0x2) >> 1) + ((col_dir & 0x4) << 1) + ((col_dir & 0x8) >> 1);
}

// returns true if obj1 and obj2 overlaps
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

// handle collisions with screen borders
static int border_collision(GameObject * object) {
    int object_top = object->y;
    int object_bottom = object->y + object->height;
    int object_right = object->x + object->width;
    int object_left = object->x;

    if (object_top <= 0) { // top
        bounce(object, 0x4);
    }
    if (object_left <= 0) { //left
        bounce(object, 0x1);
    }
    if (object_right >= DISPLAY_WIDTH) { //right
        bounce(object, 0x2);
    } 
    if (object_bottom >= DISPLAY_HEIGHT + 4) { //bottom
        if (object->type == Ball) {
            game_status.num_balls--;
        }
        remove_game_object(object);
    }
}

// check for collisions between two collision groups
void check_for_collisions(GameObject cg1[], int cg1_count, GameObject cg2[], int cg2_count) {
    for (int i = 0; i < cg1_count; i++)
    {
        if (cg1[i].collide) {
            for (int j = 0; j < cg2_count; j++)
            {
                if(cg2[j].collide) {
                    border_collision(&cg2[j]);
                    if (check_for_collision(&cg1[i], &cg2[j])) {
                        short collision_direction = get_collision_direction(&cg1[i], &cg2[j]);
                        if (cg1[i].collision_function) (*(cg1[i].collision_function))(&cg1[i], &cg2[j], collision_direction);
                        if (cg2[j].collision_function) (*(cg2[j].collision_function))(&cg2[j], &cg1[i], rev_col_dir(collision_direction)); 
                    }
                }
            }
        }
    }
}

// Object type specific collision functions

void collide_ball(GameObject * this, GameObject * other_obj, short direction) {
    switch (other_obj->type)
    {
    case Pad:
        {
            float ball_mid = this->x + this->width/2;
            float pad_mid = other_obj->x + other_obj->width/2;
            float diff = ball_mid - pad_mid;
            float velocity_mult = 1.6*(diff) / other_obj->width; // -1 to 1 to indicate direction

            if (fabs(2*diff) >= PAD_NO_CURVE) {
                
                this->speed_x = game_settings.ball_speed * velocity_mult;
                this->speed_y = -sqrt(game_settings.ball_speed*game_settings.ball_speed - this->speed_x*this->speed_x);
            } else {
                bounce(this, 0x8);
            }    
        }
        break;
    default:
        if (!other_obj->penetrable) {
            bounce(this, direction);
        }
        if (damage(this, other_obj)) {
            remove_game_object(this);
            game_status.num_balls--;
        }
        break;
    }
    #if TESTENV == 1
    printf("ball collided in direction: %d\n", direction);
    #endif
}

void collide_brick(GameObject * this, GameObject * other_obj, short direction) {
    switch (other_obj->type)
    {
    case Ball:
        if (damage(this, other_obj)) {
            remove_game_object(this);
            game_status.num_bricks--;
            if (this->subtype == 2) {
                GameObject upgrade = create_game_object(Upgrade, this->type_data, this->x, this->y, 0, UPGRADE_SPEED);
                add_game_object(upgrade, game_objects_cg2, &cg2_count);
            }
            game_status.points++;
        }
        break;
    
    default:
        break;
    }
    #if TESTENV == 1
    printf("Brick collided in direction: %d\n", direction);
    #endif
}

void activate_upgrade(short subtype) {
    switch (subtype)
    {
    case 1: // Extra Life
        game_status.lives++;
        break;
    case 2: // Fast speed
        add_buffs((GameSettings){.ball_speed = 4}, (SettingsDuration) {.ball_speed = 200});
        break;
    case 3: // high gravity 
        add_buffs((GameSettings){.gravity_acceleration = 0.2}, (SettingsDuration) {.gravity_acceleration = 50});
        break;
    case 4: // split ball
        for (int i = cg2_count; i >= 0; i--)
        {
            if (game_objects_cg2[i].type == Ball) {
                GameObject ball1 = create_game_object(Ball, 1, game_objects_cg2[i].x, game_objects_cg2[i].y, game_objects_cg2[i].speed_x, game_objects_cg2[i].speed_y);
                set_velocity(&ball1, game_settings.ball_speed, 1.2);
                set_velocity(&game_objects_cg2[i], game_settings.ball_speed, -1.2);
                add_game_object(ball1, game_objects_cg2, &cg2_count);
                game_status.num_balls++;
            }
        }
    case 5: // points
        game_status.points += 5;
    default:
        break;
    }
}

void collide_upgrade(GameObject * this, GameObject * other_obj, short direction) {
    switch (other_obj->type)
    {
    case Pad:
        activate_upgrade(this->subtype);
        remove_game_object(this);
        break;
    
    case Barrier:
        remove_game_object(this);
        break;

    default:
        break;
    }
}