#pragma once

#include "chunk.h"
#include "player.h"

#define RENDER_DISTANCE 8
#define WORLD_SIZE (RENDER_DISTANCE * 2 + 1)
#define WORLD_VOLUME (WORLD_SIZE * WORLD_SIZE)

typedef struct world_s {
    player_t *player;
    chunk_t **chunks;
} world_t;

world_t *world_create();
void world_destroy(world_t *world);

void world_update(world_t *world);
void world_render(world_t *world);

chunk_t *world_get_chunk(world_t *world, ivec2 position);
int world_get_block_id(world_t *world, vec3 position);
bool world_ray_cast(world_t *world, vec3 position, vec3 direction, float distance, ivec3 result, vec3 normal);

void world_add_block(world_t *world, ivec3 position, int id);
void world_remove_block(world_t *world, ivec3 position);