#include "gui.h"
#include "state.h"

gui_t *gui_create()
{
    gui_t *gui = malloc(sizeof(gui_t));
    if (!gui) {
        return NULL;
    }

    float vertices[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &gui->vao);
    glBindVertexArray(gui->vao);

    glGenBuffers(1, &gui->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gui->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return gui;
}

void gui_destroy(gui_t *gui)
{
    if (!gui) { return; }

    glDeleteVertexArrays(1, &gui->vao);
    glDeleteBuffers(1, &gui->vbo);
    free(gui);
}

void gui_update(gui_t *gui)
{
    (void)gui;
}

void gui_render(gui_t *gui)
{
    (void)gui;
    glBindVertexArray(gui->vao);
    texture_bind(state.renderer->textures[TEXTURE_GUI], 0);
    shader_use(state.renderer->shaders[SHADER_GUI]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}