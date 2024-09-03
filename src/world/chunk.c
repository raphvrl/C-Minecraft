#include "chunk.h"
#include "state.h"
#include "block/block.h"

static void set_data(chunk_t *chunk)
{
    for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
        for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
            for (unsigned int y = 0; y < CHUNK_HEIGHT; y++) {
                if (y == 0) {
                    chunk->data[BLOCK_INDEX(x, y, z)] = BEDROCK;
                } else if (y < 4) {
                    chunk->data[BLOCK_INDEX(x, y, z)] = DIRT;
                } else if (y == 4) {
                    chunk->data[BLOCK_INDEX(x, y, z)] = GRASS;
                } else {
                    chunk->data[BLOCK_INDEX(x, y, z)] = AIR;
                }
            }
        }
    }
}

static void save_data(chunk_t *chunk)
{
    int x = chunk->position[0];
    int z = chunk->position[2];

    char full_path[256];
    sprintf(full_path, "./%s/%d_%d.dat", CHUNK_SAVE_PATH, x, z);

    FILE *file = fopen(full_path, "wb");
    if (file == NULL) {
        return;
    }

    fwrite(chunk->data, sizeof(u8), CHUNK_VOLUME, file);
    fclose(file);
}

static bool load_data(chunk_t *chunk, const char *path)
{
    int x = chunk->position[0];
    int z = chunk->position[2];

    char full_path[256];
    sprintf(full_path, "%s/%d_%d.dat", path, x, z);

    FILE *file = fopen(full_path, "rb");
    if (file == NULL) {
        return false;
    }

    fread(chunk->data, sizeof(u8), CHUNK_VOLUME, file);
    fclose(file);

    return true;
}

chunk_t *chunk_create(ivec3 position)
{
    chunk_t *chunk = malloc(sizeof(chunk_t));
    if (chunk == NULL) {
        return NULL;
    }

    glm_ivec3_copy(position, chunk->position);

    chunk->data = calloc(CHUNK_VOLUME, sizeof(u8));
    if (chunk->data == NULL) {
        chunk_destroy(chunk);
        return NULL;
    }

    chunk->mesh = chunk_mesh_create(chunk->data);
    if (chunk->mesh == NULL) {
        chunk_destroy(chunk);
        return NULL;
    }

    if (!load_data(chunk, CHUNK_SAVE_PATH)) {
        set_data(chunk);
    }

    return chunk;
}

void chunk_destroy(chunk_t *chunk)
{
    if (chunk == NULL) {
        return;
    }

    save_data(chunk);
    chunk_mesh_destroy(chunk->mesh);
    free(chunk->data);
    free(chunk);
}

void chunk_build(chunk_t *chunk)
{
    u8 *neighbors[4] = { NULL , NULL, NULL, NULL };
    for (int i = 0; i < 4; i++) {
        if (chunk->neighbors[i] != NULL) {
            neighbors[i] = chunk->neighbors[i]->data;
        }
    }

    chunk_mesh_build(chunk->mesh, chunk->data, neighbors);
}

void chunk_update(chunk_t *chunk)
{
    if (!chunk) {
        return;
    }

    vec3 position = {
        chunk->position[0] * CHUNK_SIZE, 
        chunk->position[1], 
        chunk->position[2] * CHUNK_SIZE
    };
    glm_mat4_identity(chunk->model);
    glm_translate(chunk->model, position);
}

void chunk_render(chunk_t *chunk)
{
    texture_bind(state.renderer->textures[TEXTURE_BLOCKS], 0);
    chunk_mesh_render(chunk->mesh);
}

void chunk_change_blocks(chunk_t *chunk, ivec3 pos, int id)
{
    int index = BLOCK_INDEX(pos[0], pos[1], pos[2]);
    chunk->data[index] = id;
    chunk_build(chunk);
}

int chunk_get_block_id(chunk_t *chunk, ivec3 pos)
{
    if (pos[0] < 0 || pos[0] >= CHUNK_SIZE || pos[1] < 0 || pos[1] >= CHUNK_HEIGHT || pos[2] < 0 || pos[2] >= CHUNK_SIZE) {
        return -1;
    }

    return chunk->data[BLOCK_INDEX(pos[0], pos[1], pos[2])];
}

void chunk_update_neighbors(chunk_t *chunk, chunk_t *neighbors[4])
{
    if (chunk == NULL) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (neighbors[i] != NULL) {
            chunk->neighbors[i] = neighbors[i];
        } else {
            chunk->neighbors[i] = NULL;
        }
    }
}