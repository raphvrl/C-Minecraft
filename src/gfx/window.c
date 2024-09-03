#include "window.h"

window_t *window_create(int width, int height, const char *title)
{
    if (glfwInit() == GLFW_FALSE) {
        return NULL;
    }

    window_t *window = malloc(sizeof(window_t));
    if (window == NULL) {
        return NULL;
    }

    
    window->width = width;
    window->height = height;
    window->title = title;
    window->x = 0;
    window->y = 0;
    window->dx = 0;
    window->dy = 0;

    window->monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(window->monitor);

    window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
    if (window->window == NULL) {
        free(window);
        return NULL;
    }

    int xpos = (mode->width - window->width) / 2;
    int ypos = (mode->height - window->height) / 2;
    glfwSetWindowPos(window->window, xpos, ypos);

    glfwMakeContextCurrent(window->window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window->window);
        free(window);
        return NULL;
    }

    glViewport(0, 0, window->width, window->height);

    return window;
}

void window_destroy(window_t *window)
{
    if (window == NULL) {
        return;
    }

    glfwDestroyWindow(window->window);
    free(window);
}

void window_update(window_t *window)
{
    double xpos, ypos;
    glfwGetCursorPos(window->window, &xpos, &ypos);

    if (window->first_mouse) {
        window->x = xpos;
        window->y = ypos;
        window->first_mouse = false;
    }

    window->dx = xpos - window->x;
    window->dy = window->y - ypos;

    window->x = xpos;
    window->y = ypos;

    glfwSwapBuffers(window->window);
    glfwPollEvents();
}

int window_is_open(window_t *window)
{
    return !glfwWindowShouldClose(window->window);
}

void window_close(window_t *window)
{
    glfwSetWindowShouldClose(window->window, GLFW_TRUE);
}

bool window_is_key_pressed(window_t *window, int key)
{
    return glfwGetKey(window->window, key) == GLFW_PRESS;
}

bool window_is_mouse_pressed(window_t *window, int button)
{
    return glfwGetMouseButton(window->window, button) == GLFW_PRESS;
}

void window_cursor_mode(window_t *window, int mode)
{
    glfwSetInputMode(window->window, GLFW_CURSOR, mode);
}

float window_delta_time(window_t *window)
{
    float current_time = glfwGetTime();
    window->delta_time = current_time - window->last_time;
    window->last_time = current_time;

    return window->delta_time;
}