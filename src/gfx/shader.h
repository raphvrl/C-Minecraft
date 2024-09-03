#pragma once

#include <glad/glad.h>
#include <cglm/cglm.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "file/file.h"

#define LOG_SIZE 512

typedef struct shader_s {
    GLuint id;
} shader_t;

shader_t *shader_create();
shader_t *shader_create_from_source(const char *vertex, const char *fragment, const char *geometry);
void shader_destroy(shader_t *shader);

void shader_set_vertex(shader_t *shader, const char *path);
void shader_set_fragment(shader_t *shader, const char *path);
void shader_set_geometry(shader_t *shader, const char *path);

void shader_link(shader_t *shader);

void shader_use(shader_t *shader);
void shader_unuse();

void shader_set_bool(shader_t *shader, const char *name, bool value);
void shader_set_int(shader_t *shader, const char *name, int value);
void shader_set_float(shader_t *shader, const char *name, float value);
void shader_set_vec2(shader_t *shader, const char *name, const vec2 value);
void shader_set_vec3(shader_t *shader, const char *name, const vec3 value);
void shader_set_vec4(shader_t *shader, const char *name, const vec4 value);
void shader_set_mat4(shader_t *shader, const char *name, const mat4 value);
