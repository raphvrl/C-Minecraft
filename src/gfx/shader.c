#include "shader.h"

shader_t *shader_create()
{
    shader_t *shader = malloc(sizeof(shader_t));
    if (shader == NULL) {
        return NULL;
    }

    shader->id = glCreateProgram();

    return shader;
}

shader_t *shader_create_from_source(const char *vertex, const char *fragment, const char *geometry)
{
    shader_t *shader = shader_create();
    if (shader == NULL) {
        return NULL;
    }

    shader_set_vertex(shader, vertex);
    shader_set_fragment(shader, fragment);
    if (geometry != NULL) {
        shader_set_geometry(shader, geometry);
    }

    shader_link(shader);

    return shader;
}

void shader_destroy(shader_t *shader)
{
    if (shader == NULL) {
        return;
    }

    glDeleteProgram(shader->id);
    free(shader);
}

void shader_set_vertex(shader_t *shader, const char *path)
{
    char *source = file_read(path);
    if (source == NULL) {
        return;
    }

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char **)&source, NULL);
    glCompileShader(vertex);
    
    GLint success;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(vertex, sizeof(log), NULL, log);
        fprintf(stderr, "Failed to compile vertex %s shader: %s\n", path, log);
        return;
    }

    glAttachShader(shader->id, vertex);
    glDeleteShader(vertex);
}

void shader_set_fragment(shader_t *shader, const char *path)
{
    char *source = file_read(path);
    if (source == NULL) {
        return;
    }

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char **)&source, NULL);
    glCompileShader(fragment);
        
    GLint success;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[LOG_SIZE];
        glGetShaderInfoLog(fragment, sizeof(log), NULL, log);
        fprintf(stderr, "Failed to compile fragment %s shader: %s\n", path, log);
        return;
    }

    glAttachShader(shader->id, fragment);
    glDeleteShader(fragment);
}

void shader_set_geometry(shader_t *shader, const char *path)
{
    char *source = file_read(path);
    if (source == NULL) {
        return;
    }

    GLuint geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, (const char **)&source, NULL);
    glCompileShader(geometry);

    GLint success;
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[LOG_SIZE];
        glGetShaderInfoLog(geometry, sizeof(log), NULL, log);
        fprintf(stderr, "Failed to compile geometry %s shader: %s\n", path, log);
        return;
    }

    glAttachShader(shader->id, geometry);
    glDeleteShader(geometry);
}

void shader_link(shader_t *shader)
{
    glLinkProgram(shader->id);

    GLint success;
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if (!success) {
        char log[LOG_SIZE];
        glGetProgramInfoLog(shader->id, sizeof(log), NULL, log);
        fprintf(stderr, "Failed to link shader: %s\n", log);
        return;
    }
}

void shader_use(shader_t *shader)
{
    glUseProgram(shader->id);
}

void shader_unuse()
{
    glUseProgram(0);
}

void shader_set_bool(shader_t *shader, const char *name, bool value)
{
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_int(shader_t *shader, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_float(shader_t *shader, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

void shader_set_vec2(shader_t *shader, const char *name, const vec2 value)
{
    glUniform2fv(glGetUniformLocation(shader->id, name), 1, (float *)value);
}

void shader_set_vec3(shader_t *shader, const char *name, const vec3 value)
{
    glUniform3fv(glGetUniformLocation(shader->id, name), 1, (float *)value);
}

void shader_set_vec4(shader_t *shader, const char *name, const vec4 value)
{
    glUniform4fv(glGetUniformLocation(shader->id, name), 1, (float *)value);
}

void shader_set_mat4(shader_t *shader, const char *name, const mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, (float *)value);
}
