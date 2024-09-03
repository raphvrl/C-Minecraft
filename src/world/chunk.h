#pragma once

#include "type.h"
#include "chunk_mesh.h"
#include "gfx/texture.h"

typedef struct chunk_s {
    chunk_mesh_t *mesh;
    ivec3 position;
    mat4 model;
    bool updated;
    u8 *data;
    struct chunk_s *neighbors[4];
} chunk_t;

#define CHUNK_SAVE_PATH "./saves/chunks/"

chunk_t *chunk_create(ivec3 position);
void chunk_destroy(chunk_t *chunk);
void chunk_build(chunk_t *chunk);

void chunk_update(chunk_t *chunk);
void chunk_render(chunk_t *chunk);

void chunk_change_blocks(chunk_t *chunk, ivec3 pos, int id);
int chunk_get_block_id(chunk_t *chunk, ivec3 pos);
void chunk_update_neighbors(chunk_t *chunk, chunk_t *neighbors[4]);