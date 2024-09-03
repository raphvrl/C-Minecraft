#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <stdlib.h>
#include <stdbool.h>

typedef struct texture_s {
    GLuint id;
    int width;
    int height;
} texture_t;

texture_t *texture_create(const char *path);
void texture_destroy(texture_t *texture);

void texture_bind(texture_t *texture, GLuint slot);
void texture_unbind();