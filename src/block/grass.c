#include "block.h"

static void get_texture_uv(int face, ivec2 uv)
{
    switch (face) {
        case FACE_TOP:
            glm_ivec2_copy((ivec2) {2, 0}, uv);
            break;
        case FACE_BOTTOM:
            glm_ivec2_copy((ivec2) {3, 0}, uv);
            break;
        default:
            glm_ivec2_copy((ivec2) {1, 0}, uv);
            break;
    }
}

block_t init_grass_block()
{
    block_t block = {
        .get_texture_uv = get_texture_uv
    };

    return block;
}
