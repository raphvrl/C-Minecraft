#pragma once

#include <cglm/cglm.h>

enum FACE {
    FACE_TOP = 0,
    FACE_BOTTOM,
    FACE_LEFT,
    FACE_RIGHT,
    FACE_FRONT,
    FACE_BACK,
};

enum BLOCK_ID {
    AIR = 0,
    STONE = 1,
    GRASS = 2,
    DIRT = 3,
    COBBLESTONE = 4,
    WOOD_PLANKS = 5,
    BEDROCK = 7,
    WATER = 8,
    LAVA = 10,
    SAND = 12,
    GRAVEL = 13,
    GOLD_ORE = 14,
    IRON_ORE = 15,
    COAL_ORE = 16,
    LOG = 17,
    LEAVES = 18,
    SPONGE = 19,
    GLASS = 20,
};

#define BLOCK_COUNT 18

#define TEXTURE_SIZE 16
#define CELL_SIZE (1.0f / TEXTURE_SIZE)

typedef struct block_s {
    void (*get_texture_uv)(int face, ivec2 uv);
} block_t;

#define DECLARE_BLOCK(name) \
    block_t init_##name##_block();

#define GET_BLOCK_STRUCT(name) \
    init_##name##_block()

extern block_t block_list[BLOCK_COUNT];

void init_block_list();