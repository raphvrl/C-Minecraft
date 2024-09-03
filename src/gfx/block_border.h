#pragma once

#include <glad/glad.h>
#include "world/player.h"

typedef struct block_border_s {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} block_border_t;

block_border_t *block_border_create();
void block_border_destroy(block_border_t *border);

void block_border_render(block_border_t *border, player_t *player);