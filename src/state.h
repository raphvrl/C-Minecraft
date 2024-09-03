#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include "gfx/renderer.h"
#include "world/world.h"
#include "gui/gui.h"

typedef struct state_s {
    window_t *window;
    renderer_t *renderer;
    world_t *world;
    gui_t *gui;

    float dt;
} state_t;

extern state_t state;