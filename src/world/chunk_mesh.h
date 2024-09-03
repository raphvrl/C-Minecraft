#pragma once

#include <glad/glad.h>
#include <cglm/cglm.h>

#include <stdlib.h>

#include "type.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 16
#define CHUNK_VOLUME (CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT)

#define BLOCK_INDEX(x, y, z) ((x) + (z) * CHUNK_SIZE + (y) * CHUNK_SIZE * CHUNK_SIZE)

enum CHUNK_FACE {
    CHUNK_LEFT = 0,
    CHUNK_RIGHT,
    CHUNK_FRONT,
    CHUNK_BACK
};

typedef struct vertex_s {
    vec3 position;
    vec2 uv;
} vertex_t;

typedef struct chunk_mesh_s {
    GLuint vao;
    GLuint vbo;

    vertex_t *vertices;
    size_t vertex_count;
} chunk_mesh_t;

chunk_mesh_t *chunk_mesh_create();
void chunk_mesh_destroy(chunk_mesh_t *mesh);

void chunk_mesh_render(chunk_mesh_t *mesh);

void chunk_mesh_build(chunk_mesh_t *mesh, u8 *data, u8 *neighbors[4]);