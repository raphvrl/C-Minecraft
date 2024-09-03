#pragma once

#include "camera.h"
#include "math/div.h"

#define PLAYER_WALK_SPEED 4.317f
#define PLAYER_PICK_RANGE 3.0f 

#define PLAYER_UP (vec3){0.0f, 1.0f, 0.0f}

typedef struct player_s {
    camera_t *camera;
    ivec3 chunk_position;

    float speed;
    vec3 walk_direction;

    vec3 ray;
    float click_couldown;
} player_t;

player_t *player_create();
void player_destroy(player_t *player);

void player_update(player_t *player);