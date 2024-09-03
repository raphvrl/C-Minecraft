#include "renderer.h"

renderer_t *renderer_create()
{
    renderer_t *renderer = malloc(sizeof(renderer_t));
    if (!renderer) {
        return NULL;
    }

    renderer->shaders[SHADER_BASIC] = shader_create_from_source("assets/shaders/default.vert", "assets/shaders/default.frag", NULL);
    renderer->shaders[SHADER_GUI] = shader_create_from_source("assets/shaders/gui.vert", "assets/shaders/gui.frag", NULL);
    renderer->shaders[SHADER_BORDER] = shader_create_from_source("assets/shaders/border.vert", "assets/shaders/border.frag", NULL);

    renderer->textures[TEXTURE_BLOCKS] = texture_create("assets/img/blocks.png");
    renderer->textures[TEXTURE_GUI] = texture_create("assets/img/cross.png");

    renderer->block_border = block_border_create();

    return renderer;
}

void renderer_destroy(renderer_t *renderer)
{
    if (!renderer) {
        return;
    }

    for (unsigned int i = 0; i < SHADER_COUNT; i++) {
        shader_destroy(renderer->shaders[i]);
    }

    for (unsigned int i = 0; i < TEXTURE_COUNT; i++) {
        texture_destroy(renderer->textures[i]);
    }

    block_border_destroy(renderer->block_border);

    free(renderer);
}