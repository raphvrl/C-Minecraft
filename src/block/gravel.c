#include "block/block.h"

static void get_texture_uv(int face, ivec2 uv)
{
    (void)face;
    glm_ivec2_copy((ivec2) {8, 0}, uv);
}

block_t init_gravel_block()
{
    block_t block = {
        .get_texture_uv = get_texture_uv
    };

    return block;
}