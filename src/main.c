#include "state.h"

state_t state;

static void destroy_save()
{
    struct dirent *entry;
    DIR *dir = opendir("saves/chunks/");
    if (!dir) {
        return;
    }

    while ((entry = readdir(dir))) {
        if (strstr(entry->d_name, ".dat")) {
            char path[256];
            sprintf(path, "saves/chunks/%s", entry->d_name);
            remove(path);
        }
    }

}

void update()
{
    if (window_is_key_pressed(state.window, GLFW_KEY_ESCAPE)) {
        window_close(state.window);
    }

    state.dt = window_delta_time(state.window);

    world_update(state.world);
}

void render()
{
    glClearColor(0.47f, 0.65f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world_render(state.world);

    block_border_render(state.renderer->block_border, state.world->player);

    gui_render(state.gui);

    window_update(state.window);
}

void init()
{
    state.window = window_create(1280, 720, "CMinecraft");
    state.renderer = renderer_create();
    state.world = world_create();
    state.gui = gui_create();

    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    window_cursor_mode(state.window, GLFW_CURSOR_DISABLED);

    glEnable(GL_CULL_FACE);
}

void destroy()
{
    renderer_destroy(state.renderer);
    window_destroy(state.window);
    world_destroy(state.world);
    gui_destroy(state.gui);
    destroy_save();
}

void loop()
{
    while (window_is_open(state.window)) {
        update();
        render();
    }
}

int main(void)
{
    init();
    loop();
    destroy();
    return 0;
}