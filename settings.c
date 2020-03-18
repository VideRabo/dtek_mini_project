#include "settings.h"

// this part of the code is an absolute disgarce, please procede with caution and keep a bottle of bleach at hand.


static GameSettings settings_backup;
static SettingsDuration settings_duration;


// must be called on startup
void reset_settings() {
    settings_backup = (GameSettings) {
        PAD_SIZE,
        BALL_SIZE,
        BALL_SPEED,
        AIR_RESISTANCE,
        AR_MIN_VELOCITY,
        GRAVITY_ACCELERATION
    };

    settings_duration = (SettingsDuration) {
        -1, -1, -1, -1, -1, -1
    };

    copy_settings(&game_settings, &settings_backup);
}

static void copy_changed_settings(GameSettings * destination, GameSettings * source) {
    destination->pad_size =                source->pad_size ? source->pad_size : destination->pad_size;
    destination->ball_size =               source->ball_size ? source->ball_size : destination->ball_size;
    destination->ball_speed =              source->ball_speed ? source->ball_speed : destination->ball_speed;
    destination->air_resistance =          source->air_resistance ? source->air_resistance : destination->air_resistance;
    destination->ar_minimum_velocity =     source->ar_minimum_velocity ? source->ar_minimum_velocity : destination->ar_minimum_velocity;
    destination->gravity_acceleration =    source->gravity_acceleration ? source->gravity_acceleration : destination->gravity_acceleration;
}

static void copy_buffs(GameSettings * destination, GameSettings * source, SettingsDuration * duration) {
    destination->pad_size =                duration->pad_size ? source->pad_size : destination->pad_size;
    destination->ball_size =               duration->ball_size ? source->ball_size : destination->ball_size;
    destination->ball_speed =              duration->ball_speed ? source->ball_speed : destination->ball_speed;
    destination->air_resistance =          duration->air_resistance ? source->air_resistance : destination->air_resistance;
    destination->ar_minimum_velocity =     duration->ar_minimum_velocity ? source->ar_minimum_velocity : destination->ar_minimum_velocity;
    destination->gravity_acceleration =    duration->gravity_acceleration ? source->gravity_acceleration : destination->gravity_acceleration;
}

static void copy_changed_duration(SettingsDuration * destination, SettingsDuration * source) {
    destination->pad_size =                source->pad_size ? source->pad_size : destination->pad_size;
    destination->ball_size =               source->ball_size ? source->ball_size : destination->ball_size;
    destination->ball_speed =              source->ball_speed ? source->ball_speed : destination->ball_speed;
    destination->air_resistance =          source->air_resistance ? source->air_resistance : destination->air_resistance;
    destination->ar_minimum_velocity =     source->ar_minimum_velocity ? source->ar_minimum_velocity : destination->ar_minimum_velocity;
    destination->gravity_acceleration =    source->gravity_acceleration ? source->gravity_acceleration : destination->gravity_acceleration;
}

void update_settings(GameSettings new_settings, short ignore_zeros) {
    if(ignore_zeros) {
        copy_changed_settings(&settings_backup, &new_settings);
    } else {
        copy_settings(&settings_backup, &new_settings);
    }

    copy_settings(&game_settings, &settings_backup);
}

void copy_settings(GameSettings * destination, GameSettings * source) {
    *destination = (GameSettings) {
        source->pad_size,
        source->ball_size,
        source->ball_speed,
        source->air_resistance,
        source->ar_minimum_velocity,
        source->gravity_acceleration
    };
}

void update_buffs() {
    if(settings_duration.pad_size > -1 && !settings_duration.pad_size--) game_settings.pad_size = settings_backup.pad_size;
    if(settings_duration.ball_size > -1 && !settings_duration.ball_size--) game_settings.ball_size = settings_backup.ball_size;
    if(settings_duration.ball_speed > -1 && !settings_duration.ball_speed--) game_settings.ball_speed = settings_backup.ball_speed;
    if(settings_duration.air_resistance > -1 && !settings_duration.air_resistance--) game_settings.air_resistance = settings_backup.air_resistance;
    if(settings_duration.ar_minimum_velocity > -1 && !settings_duration.ar_minimum_velocity--) game_settings.ar_minimum_velocity = settings_backup.ar_minimum_velocity;
    if(settings_duration.gravity_acceleration > -1 && !settings_duration.gravity_acceleration--) game_settings.gravity_acceleration = settings_backup.gravity_acceleration;
}

void add_buffs(GameSettings buffs, SettingsDuration duration) {
    copy_buffs(&game_settings, &buffs, &duration);
    copy_changed_duration(&settings_duration, &duration);
}

void remove_buffs() {
    copy_settings(&game_settings, &settings_backup);
    
    settings_duration = (SettingsDuration) {
        -1, -1, -1, -1, -1, -1
    };
}

void d_print_settings() {
    #if TESTENV == 1
    printf("### game_settings ###\n");
    printf("(%d) : pad_size %d\n", settings_duration.pad_size, game_settings.pad_size);
    printf("(%d) : ball_size %d\n", settings_duration.ball_size, game_settings.ball_size);
    printf("(%d) : ball_speed %f\n", settings_duration.ball_speed, game_settings.ball_speed);
    printf("(%d) : air_resistance %f\n", settings_duration.air_resistance, game_settings.air_resistance);
    printf("(%d) : ar_minimum_velocity %f\n", settings_duration.ar_minimum_velocity, game_settings.ar_minimum_velocity);
    printf("(%d) : gravity_acceleration %f\n", settings_duration.gravity_acceleration, game_settings.gravity_acceleration);
    #endif
}