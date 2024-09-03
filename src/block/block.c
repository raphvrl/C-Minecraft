#include "block.h"

block_t block_list[BLOCK_COUNT];

void init_block_list()
{
    DECLARE_BLOCK(grass);
    DECLARE_BLOCK(dirt);
    DECLARE_BLOCK(stone);
    DECLARE_BLOCK(cobblestone);
    DECLARE_BLOCK(plank);
    DECLARE_BLOCK(bedrock);
    DECLARE_BLOCK(sand);
    DECLARE_BLOCK(gravel);

    block_list[STONE] = GET_BLOCK_STRUCT(stone);
    block_list[GRASS] = GET_BLOCK_STRUCT(grass);
    block_list[DIRT] = GET_BLOCK_STRUCT(dirt);
    block_list[COBBLESTONE] = GET_BLOCK_STRUCT(cobblestone);
    block_list[WOOD_PLANKS] = GET_BLOCK_STRUCT(plank);
    block_list[BEDROCK] = GET_BLOCK_STRUCT(bedrock);
    block_list[SAND] = GET_BLOCK_STRUCT(sand);
    block_list[GRAVEL] = GET_BLOCK_STRUCT(gravel);
}
