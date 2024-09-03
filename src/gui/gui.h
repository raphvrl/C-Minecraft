#pragma once

#include <glad/glad.h>

typedef struct gui_s {
    GLuint vao;
    GLuint vbo;
} gui_t;

gui_t *gui_create();
void gui_destroy(gui_t *gui);

void gui_update(gui_t *gui);
void gui_render(gui_t *gui);
