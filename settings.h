#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdio.h>
#include "config.h"


// TYPES

typedef struct {
    short pad_size; // not used
    short ball_size; 
    float ball_speed; // not used
    float air_resistance;
    float ar_minimum_velocity; 
    float gravity_acceleration; 
} GameSettings;

// keeps track of when to remove a buff. Values decrease every tick ( update_buffs() ) and the setting will revert when reaching 0
typedef struct { 
    int pad_size;
    int ball_size; 
    int ball_speed;
    int air_resistance;
    int ar_minimum_velocity; 
    int gravity_acceleration; 
} SettingsDuration;


// PUBLIC VARIABLES

// global settings - other files acesses settings through this object
GameSettings game_settings;


// PUBLIC FUNCTIONS

// resets game_settings to default settings (in config.h) - should be called on startup
void reset_settings();

// updates game_settings with non-NULL properties in new_settings. If ignore zeros is true, non-initialized values and 0 will not change the settings
void update_settings(GameSettings new_settings, short ignore_zeros);

// copies properites of settings from source to destination
void copy_settings(GameSettings * destination, GameSettings * source);

// debug. print settings to console
void d_print_settings();

// should be called every game tick - ensures buffs are eventually removed
void update_buffs();

// adds buffs (duh). duration must be set for a corresponding setting. -1 gives unlimited duration
void add_buffs(GameSettings buff, SettingsDuration duration);

// it... removes buffs
void remove_buffs();

#endif