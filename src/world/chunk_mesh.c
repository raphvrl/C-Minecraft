#include "chunk_mesh.h"
#include "block/block.h"

static bool is_void(u8 *data, u8 *neighbors[4], int x, int y, int z)
{
    if (y < 0 || y >= CHUNK_HEIGHT) {
        return true;
    }

    if (x < 0) {
        return neighbors[CHUNK_LEFT] != NULL && neighbors[CHUNK_LEFT][BLOCK_INDEX(CHUNK_SIZE - 1, y, z)] == 0;
    }
    
    if (x >= CHUNK_SIZE) {
        return neighbors[CHUNK_RIGHT] != NULL && neighbors[CHUNK_RIGHT][BLOCK_INDEX(0, y, z)] == 0;
    }
    
    if (z < 0) {
        return neighbors[CHUNK_FRONT] != NULL && neighbors[CHUNK_FRONT][BLOCK_INDEX(x, y, CHUNK_SIZE - 1)] == 0;
    }
    
    if (z >= CHUNK_SIZE) {
        return neighbors[CHUNK_BACK] != NULL && neighbors[CHUNK_BACK][BLOCK_INDEX(x, y, 0)] == 0;
    }

    return data[BLOCK_INDEX(x, y, z)] == 0;
}

chunk_mesh_t *chunk_mesh_create()
{
    chunk_mesh_t *mesh = malloc(sizeof(chunk_mesh_t));
    if (mesh == NULL) {
        return NULL;
    }

    glGenBuffers(1, &mesh->vbo);
    glGenVertexArrays(1, &mesh->vao);

    mesh->vertices = malloc(sizeof(vertex_t) * CHUNK_VOLUME * 6 * 6);
    mesh->vertex_count = 0;

    return mesh;
}

void chunk_mesh_destroy(chunk_mesh_t *mesh)
{
    if (mesh == NULL) {
        return;
    }

    glDeleteBuffers(1, &mesh->vbo);
    glDeleteVertexArrays(1, &mesh->vao);
    free(mesh->vertices);
    free(mesh);
}

void chunk_mesh_render(chunk_mesh_t *mesh)
{
    glBindVertexArray(mesh->vao);

    glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);

    glBindVertexArray(0);
}

void chunk_mesh_build(chunk_mesh_t *mesh, u8 *data, u8 *neighbors[4])
{
    mesh->vertex_count = 0;
    int index = 0;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y < CHUNK_HEIGHT; y++) {

                int id = data[BLOCK_INDEX(x, y, z)];

                if (id == 0) {
                    continue;
                }

                if (is_void(data, neighbors, x, y + 1, z)) {
                    ivec2 uv;
                    block_list[id].get_texture_uv(FACE_TOP, uv);

                    float u_min = 16 * CELL_SIZE + uv[0] * CELL_SIZE;
                    float v_min = uv[1] * CELL_SIZE;
                    float u_max = u_min + CELL_SIZE;
                    float v_max = v_min - CELL_SIZE;

                    float uvs[6][2] = {
                        {u_min, v_max},
                        {u_min, v_min},
                        {u_max, v_max},
                        {u_max, v_max},
                        {u_min, v_min},
                        {u_max, v_min},
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z},
                        .uv = {uvs[0][0], uvs[0][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z + 1},
                        .uv = {uvs[1][0], uvs[1][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y + 1, z},
                        .uv = {uvs[2][0], uvs[2][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y + 1, z},
                        .uv = {uvs[3][0], uvs[3][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z + 1},
                        .uv = {uvs[4][0], uvs[4][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y + 1, z + 1},
                        .uv = {uvs[5][0], uvs[5][1]}
                    };
                }

                if (is_void(data, neighbors, x, y - 1, z)) {
                    ivec2 uv;
                    block_list[id].get_texture_uv(FACE_BOTTOM, uv);

                    float u_min = 16 * CELL_SIZE + uv[0] * CELL_SIZE;
                    float v_min = uv[1] * CELL_SIZE;
                    float u_max = u_min + CELL_SIZE;
                    float v_max = v_min - CELL_SIZE;

                    float uvs[6][2] = {
                        {u_min, v_max},
                        {u_min, v_min},
                        {u_max, v_max},
                        {u_max, v_max},
                        {u_min, v_min},
                        {u_max, v_min},
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y, z},
                        .uv = {uvs[0][0], uvs[0][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z},
                        .uv = {uvs[1][0], uvs[1][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y, z + 1},
                        .uv = {uvs[2][0], uvs[2][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y, z + 1},
                        .uv = {uvs[3][0], uvs[3][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z},
                        .uv = {uvs[4][0], uvs[4][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z + 1},
                        .uv = {uvs[5][0], uvs[5][1]}
                    };
                }

                if (is_void(data, neighbors, x, y, z - 1)) {
                    ivec3 uv;
                    block_list[id].get_texture_uv(FACE_FRONT, uv);

                    float u_min = 16 * CELL_SIZE + uv[0] * CELL_SIZE;
                    float v_min = uv[1] * CELL_SIZE;
                    float u_max = u_min + CELL_SIZE;
                    float v_max = v_min - CELL_SIZE;

                    float uvs[6][2] = {
                        {u_max, v_max},
                        {u_max, v_min},
                        {u_min, v_max},
                        {u_min, v_max},
                        {u_max, v_min},
                        {u_min, v_min},
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y, z},
                        .uv = {uvs[0][0], uvs[0][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z},
                        .uv = {uvs[1][0], uvs[1][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z},
                        .uv = {uvs[2][0], uvs[2][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z},
                        .uv = {uvs[3][0], uvs[3][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z},
                        .uv = {uvs[4][0], uvs[4][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y + 1, z},
                        .uv = {uvs[5][0], uvs[5][1]}
                    };
                }

                if (is_void(data, neighbors, x, y, z + 1)) {
                    ivec2 uv;
                    block_list[id].get_texture_uv(FACE_BACK, uv);

                    float u_min = 16 * CELL_SIZE + uv[0] * CELL_SIZE;
                    float v_min = uv[1] * CELL_SIZE;
                    float u_max = u_min + CELL_SIZE;
                    float v_max = v_min - CELL_SIZE;

                    float uvs[6][2] = {
                        {u_min, v_max},
                        {u_max, v_max},
                        {u_min, v_min},
                        {u_min, v_min},
                        {u_max, v_max},
                        {u_max, v_min},
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y, z + 1},
                        .uv = {uvs[0][0], uvs[0][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z + 1},
                        .uv = {uvs[1][0], uvs[1][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z + 1},
                        .uv = {uvs[2][0], uvs[2][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z + 1},
                        .uv = {uvs[3][0], uvs[3][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z + 1},
                        .uv = {uvs[4][0], uvs[4][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y + 1, z + 1},
                        .uv = {uvs[5][0], uvs[5][1]}
                    };
                }

                if (is_void(data, neighbors, x - 1, y, z)) {
                    ivec2 uv;
                    block_list[id].get_texture_uv(FACE_LEFT, uv);

                    float u_min = 16 * CELL_SIZE + uv[0] * CELL_SIZE;
                    float v_min = uv[1] * CELL_SIZE;
                    float u_max = u_min + CELL_SIZE;
                    float v_max = v_min - CELL_SIZE;

                    float uvs[6][2] = {
                        {u_min, v_max},
                        {u_max, v_max},
                        {u_min, v_min},
                        {u_min, v_min},
                        {u_max, v_max},
                        {u_max, v_min},
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y, z},
                        .uv = {uvs[0][0], uvs[0][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y, z + 1},
                        .uv = {uvs[1][0], uvs[1][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z},
                        .uv = {uvs[2][0], uvs[2][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z},
                        .uv = {uvs[3][0], uvs[3][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y, z + 1},
                        .uv = {uvs[4][0], uvs[4][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x, y + 1, z + 1},
                        .uv = {uvs[5][0], uvs[5][1]}
                    };
                }

                if (is_void(data, neighbors, x + 1, y, z)) {
                    ivec2 uv;
                    block_list[id].get_texture_uv(FACE_FRONT, uv);

                    float u_min = 16 * CELL_SIZE + uv[0] * CELL_SIZE;
                    float v_min = uv[1] * CELL_SIZE;
                    float u_max = u_min + CELL_SIZE;
                    float v_max = v_min - CELL_SIZE;

                    float uvs[6][2] = {
                        {u_max, v_max},
                        {u_max, v_min},
                        {u_min, v_max},
                        {u_min, v_max},
                        {u_max, v_min},
                        {u_min, v_min},
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z},
                        .uv = {uvs[0][0], uvs[0][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y + 1, z},
                        .uv = {uvs[1][0], uvs[1][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z + 1},
                        .uv = {uvs[2][0], uvs[2][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y, z + 1},
                        .uv = {uvs[3][0], uvs[3][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y + 1, z},
                        .uv = {uvs[4][0], uvs[4][1]}
                    };

                    mesh->vertices[index++] = (vertex_t) {
                        .position = {x + 1, y + 1, z + 1},
                        .uv = {uvs[5][0], uvs[5][1]}
                    };
                }
            }
        }
    }

    mesh->vertex_count = index;

    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * mesh->vertex_count, mesh->vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)offsetof(vertex_t, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)offsetof(vertex_t, uv));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}