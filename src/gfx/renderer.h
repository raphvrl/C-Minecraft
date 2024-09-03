#ifndef _RENDERER_H
#define _RENDERER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "block_border.h"

#define SHADER_COUNT 3
#define TEXTURE_COUNT 2

enum SHADER_ID {
    SHADER_BASIC = 0,
    SHADER_GUI,
    SHADER_BORDER,
};

enum TEXTURE_ID {
    TEXTURE_BLOCKS = 0,
    TEXTURE_GUI,
};

typedef struct renderer_s {
    shader_t *shaders[SHADER_COUNT];
    texture_t *textures[TEXTURE_COUNT];
    block_border_t *block_border;
} renderer_t;

renderer_t *renderer_create();
void renderer_destroy(renderer_t *renderer);

#endif