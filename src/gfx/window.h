#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdbool.h>

typedef struct window_s {
    GLFWwindow *window;
    GLFWmonitor *monitor;

    int width;
    int height;
    const char *title;

    float last_time;
    float delta_time;

    float x;
    float y;
    float dx;
    float dy;

    bool first_mouse;
} window_t;

window_t *window_create(int width, int height, const char *title);
void window_destroy(window_t *window);

void window_update(window_t *window);

int window_is_open(window_t *window);
void window_close(window_t *window);

bool window_is_key_pressed(window_t *window, int key);
bool window_is_mouse_pressed(window_t *window, int button);

float window_delta_time(window_t *window);

void window_cursor_mode(window_t *window, int mode);