#include "world.h"
#include "block/block.h"
#include "state.h"


static void set_neightbor(world_t *world, chunk_t *chunk)
{
    if (!chunk) {
        return;
    }

    int x = chunk->position[0];
    int z = chunk->position[2];

    int inverse[4] = { CHUNK_RIGHT, CHUNK_LEFT, CHUNK_BACK, CHUNK_FRONT };

    for (int i = 0; i < 4; i++) {
        int nx = x;
        int nz = z;

        if (i == CHUNK_LEFT) {
            nx--;
        } else if (i == CHUNK_RIGHT) {
            nx++;
        } else if (i == CHUNK_FRONT) {
            nz--;
        } else if (i == CHUNK_BACK) {
            nz++;
        }

        chunk_t *neighbor = world_get_chunk(world, (ivec2){ nx, nz });
        chunk->neighbors[i] = neighbor;
        if (neighbor != NULL) {
            int dir = inverse[i];
            neighbor->neighbors[dir] = chunk;
            chunk_build(neighbor);
        }
    }

    chunk_build(chunk);
}

static void destroy_neightbor(world_t *world, int x, int y)
{
    int inverse[4] = { CHUNK_RIGHT, CHUNK_LEFT, CHUNK_BACK, CHUNK_FRONT };

    for (int i = 0; i < 4; i++) {
        int nx = x + (i == CHUNK_LEFT) - (i == CHUNK_RIGHT);
        int nz = y + (i == CHUNK_FRONT) - (i == CHUNK_BACK);

        chunk_t *neighbor = world_get_chunk(world, (ivec2){ nx, nz });
        if (neighbor) {
            int dir = inverse[i];
            neighbor->neighbors[dir] = NULL;
            chunk_build(neighbor);
        }
    }
}

static void init_chunks(world_t *world)
{
    world->chunks = calloc(WORLD_VOLUME, sizeof(chunk_t *));
    if (!world->chunks) {
        return;
    }

    for (int i = 0; i < WORLD_VOLUME; i++) {
        int x = i / WORLD_SIZE - RENDER_DISTANCE;
        int z = i % WORLD_SIZE - RENDER_DISTANCE;

        world->chunks[i] = chunk_create((ivec3){ x, 0, z });
        set_neightbor(world, world->chunks[i]);
    }
}

static bool chunk_position_available(world_t *world, ivec2 start, ivec2 end, ivec3 result)
{
    for (int x = start[0]; x < end[0]; x++) {
        for (int z = start[1]; z < end[1]; z++) {
            ivec2 pos = { x, z };
            if (!world_get_chunk(world, pos)) {
                glm_ivec3_copy((ivec3){ x, 0, z }, result);
                return true;
            }
        }
    }

    return false;
}

world_t *world_create()
{
    world_t *world = malloc(sizeof(world_t));
    if (!world) {
        return NULL;
    }

    world->player = player_create();
    if (!world->player) {
        world_destroy(world);
        return NULL;
    }

    init_block_list();
    init_chunks(world);

    return world;
}

void world_destroy(world_t *world)
{
    if (!world) {
        return;
    }

    for (int i = 0; i < WORLD_VOLUME; i++) {
        chunk_destroy(world->chunks[i]);
    }
    player_destroy(world->player);
    free(world);
}

void world_update(world_t *world)
{
    player_update(world->player);

    ivec3 chunk_player;
    ivec2 chunk_start, chunk_end;

    glm_ivec3_copy(world->player->chunk_position, chunk_player);

    chunk_start[0] = chunk_player[0] - RENDER_DISTANCE;
    chunk_start[1] = chunk_player[2] - RENDER_DISTANCE;

    chunk_end[0] = chunk_player[0] + RENDER_DISTANCE;
    chunk_end[1] = chunk_player[2] + RENDER_DISTANCE;

    for (int i = 0; i < WORLD_VOLUME; i++) {

        if (world->chunks[i]) {
            int x = world->chunks[i]->position[0];
            int z = world->chunks[i]->position[2];

            if (x < chunk_start[0] || x > chunk_end[0] || z < chunk_start[1] || z > chunk_end[1]) {
                chunk_destroy(world->chunks[i]);
                world->chunks[i] = NULL;
                destroy_neightbor(world, x, z);
            }
        }

        if (!world->chunks[i]) {
            ivec3 new_pos = {0}; 
            if (chunk_position_available(world, chunk_start, chunk_end, new_pos)) {
                world->chunks[i] = chunk_create(new_pos);
                set_neightbor(world, world->chunks[i]);
            }
        }

        chunk_update(world->chunks[i]);
    }
}

void world_render(world_t *world)
{
    shader_use(state.renderer->shaders[SHADER_BASIC]);

    shader_set_mat4(state.renderer->shaders[SHADER_BASIC], "u_view", world->player->camera->view);
    shader_set_mat4(state.renderer->shaders[SHADER_BASIC], "u_projection", world->player->camera->projection);

    for (int i = 0; i < WORLD_VOLUME; i++) {
        chunk_t *chunk = world->chunks[i];
        if (chunk) {
            shader_set_mat4(state.renderer->shaders[SHADER_BASIC], "u_model", chunk->model);
            chunk_render(chunk);
        }
    }
}

static chunk_t *world_get_block_pos(world_t *world, vec3 position, ivec3 result)
{
    if (position[1] < 0 || position[1] >= CHUNK_HEIGHT) {
        return NULL;
    }

    int chunk_x = (int)floor(position[0] / CHUNK_SIZE);
    int chunk_z = (int)floor(position[2] / CHUNK_SIZE);

    chunk_t *chunk = world_get_chunk(world, (ivec2){ chunk_x, chunk_z });
    if (!chunk) {
        return NULL;
    }

    result[0] = (int)floor(position[0]) - chunk_x * CHUNK_SIZE;
    result[1] = (int)floor(position[1]);
    result[2] = (int)floor(position[2]) - chunk_z * CHUNK_SIZE;

    return chunk;
}

chunk_t *world_get_chunk(world_t *world, ivec2 pos)
{
    for (int i = 0; i < WORLD_VOLUME; i++) {
        chunk_t *chunk = world->chunks[i];
        if (!chunk) {
            continue;
        }

        int x = chunk->position[0];
        int z = chunk->position[2];

        if (x == pos[0] && z == pos[1]) {
            return chunk;
        }
    }

    return NULL;
}


int world_get_block_id(world_t *world, vec3 position)
{
    ivec3 block_pos;
    chunk_t *chunk = world_get_block_pos(world, position, block_pos);
    if (!chunk) {
        return 0;
    }

    return chunk_get_block_id(chunk, block_pos);
}

bool world_ray_cast(world_t *world, vec3 position, vec3 direction, float distance, ivec3 result, vec3 normal)
{
    ivec3 step;
    vec3 t_max, t_delta;
    ivec3 block = { (int)floor(position[0]), (int)floor(position[1]), (int)floor(position[2]) };

    for (int i = 0; i < 3; i++) {
        if (direction[i] > 0) {
            step[i] = 1;
            t_max[i] = (block[i] + 1 - position[i]) / direction[i];
            t_delta[i] = 1 / direction[i];
        } else {
            step[i] = -1;
            t_max[i] = (position[i] - block[i]) / -direction[i];
            t_delta[i] = 1 / -direction[i];
        }
    }

    while (distance > 0) {
        if (t_max[0] < t_max[1]) {
            if (t_max[0] < t_max[2]) {
                block[0] += step[0];
                distance -= t_max[0];
                t_max[0] += t_delta[0];
                normal[0] = -step[0];
                normal[1] = 0;
                normal[2] = 0;
            } else {
                block[2] += step[2];
                distance -= t_max[2];
                t_max[2] += t_delta[2];
                normal[0] = 0;
                normal[1] = 0;
                normal[2] = -step[2];
            }
        } else {
            if (t_max[1] < t_max[2]) {
                block[1] += step[1];
                distance -= t_max[1];
                t_max[1] += t_delta[1];
                normal[0] = 0;
                normal[1] = -step[1];
                normal[2] = 0;
            } else {
                block[2] += step[2];
                distance -= t_max[2];
                t_max[2] += t_delta[2];
                normal[0] = 0;
                normal[1] = 0;
                normal[2] = -step[2];
            }
        }

        if (world_get_block_id(world, (vec3){ block[0], block[1], block[2] })) {
            result[0] = block[0];
            result[1] = block[1];
            result[2] = block[2];
            return true;
        }
    }

    return false;
}

void world_add_block(world_t *world, ivec3 position, int id)
{
    ivec3 block_pos;
    chunk_t *chunk = world_get_block_pos(
        world, 
        (vec3){ position[0], position[1], position[2] }, 
        block_pos
    );

    if (!chunk) {
        return;
    }

    chunk_change_blocks(chunk, block_pos, id);
}

void world_remove_block(world_t *world, ivec3 position)
{
    ivec3 block_pos;
    chunk_t *chunk = world_get_block_pos(
        world, 
        (vec3){ position[0], position[1], position[2] }, 
        block_pos
    );

    if (!chunk) {
        return;
    }

    chunk_change_blocks(chunk, block_pos, AIR);
}