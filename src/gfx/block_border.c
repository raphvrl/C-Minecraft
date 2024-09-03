#include "block_border.h"
#include "state.h"

block_border_t *block_border_create()
{
    block_border_t *border = malloc(sizeof(block_border_t));
    if (!border) {
        return NULL;
    }

    float min = -0.0001f;
    float max = 1.0001f;

    float vertices[] = {
        min, min, min,
        max, min, min,
        max, max, min,
        min, max, min,

        min, min, max,
        max, min, max,
        max, max, max,
        min, max, max
    };

    unsigned int indices[] = {
        // Face avant
        0, 1, 2,
        2, 3, 0,

        // Face arrière
        4, 5, 6,
        6, 7, 4,

        // Face gauche
        0, 4, 7,
        7, 3, 0,

        // Face droite
        1, 5, 6,
        6, 2, 1,

        // Face du dessus
        3, 2, 6,
        6, 7, 3,

        // Face du dessous
        0, 1, 5,
        5, 4, 0
    };

    glGenVertexArrays(1, &border->vao);
    glGenBuffers(1, &border->vbo);
    glGenBuffers(1, &border->ebo);

    glBindVertexArray(border->vao);

    glBindBuffer(GL_ARRAY_BUFFER, border->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return border;
}

void block_border_destroy(block_border_t *border)
{
    if (!border) {
        return;
    }

    glDeleteBuffers(1, &border->vbo);
    glDeleteBuffers(1, &border->ebo);
    glDeleteVertexArrays(1, &border->vao);

    free(border);
}

void block_border_render(block_border_t *border, player_t *player)
{
    if (!border || !player) {
        return;
    }

    glDisable(GL_CULL_FACE);

    ivec3 point;
    vec3 normal;
    if (world_ray_cast(state.world, player->camera->position, player->camera->front, 3.0f, point, normal)) {
        vec3 position = { point[0], point[1], point[2] };
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        glm_translate(model, position);

        shader_use(state.renderer->shaders[SHADER_BORDER]);

        shader_set_mat4(state.renderer->shaders[SHADER_BORDER], "u_projection", player->camera->projection);
        shader_set_mat4(state.renderer->shaders[SHADER_BORDER], "u_view", player->camera->view);
        shader_set_mat4(state.renderer->shaders[SHADER_BORDER], "u_model", model);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border->ebo);
        glBindVertexArray(border->vao);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glEnable(GL_CULL_FACE);
}