#include "player.h"
#include "state.h"

player_t *player_create()
{
    player_t *player = malloc(sizeof(player_t));

    player->camera = camera_create();
    player->camera->position[0] = 0.0f;
    player->camera->position[1] = 5.62f;
    player->camera->position[2] = 0.0f;

    player->speed = PLAYER_WALK_SPEED;

    player->click_couldown = 0.0f;

    return player;
}

void player_destroy(player_t *player)
{
    if (!player)
        return;

    camera_destroy(player->camera);
    free(player);
}

static void calulate_new_block(ivec3 point, vec3 normal, ivec3 new_pos)
{
    new_pos[0] = (int)point[0] + normal[0];
    new_pos[1] = (int)point[1] + normal[1];
    new_pos[2] = (int)point[2] + normal[2];
} 

static void calulate_direction(float yaw, vec3 direction)
{
    direction[0] = cos(glm_rad(yaw));
    direction[1] = 0.0f;
    direction[2] = sin(glm_rad(yaw));
    glm_normalize(direction);
}

static bool is_picking_block(ivec3 point, vec3 normal)
{
    return world_ray_cast(
        state.world, 
        state.world->player->camera->position, 
        state.world->player->camera->front, 
        PLAYER_PICK_RANGE, 
        point, normal
    );
}

void player_update(player_t *player)
{
    calulate_direction(player->camera->yaw, player->walk_direction);

    if (window_is_key_pressed(state.window, GLFW_KEY_W)) {
        camera_move(player->camera, player->walk_direction, player->speed * state.dt);
    }

    if (window_is_key_pressed(state.window, GLFW_KEY_S)) {
        camera_move(player->camera, player->walk_direction, -player->speed * state.dt);
    }

    if (window_is_key_pressed(state.window, GLFW_KEY_A)) {
        camera_move(player->camera, player->camera->right, -player->speed * state.dt);
    }

    if (window_is_key_pressed(state.window, GLFW_KEY_D)) {
        camera_move(player->camera, player->camera->right, player->speed * state.dt);
    }

    if (window_is_key_pressed(state.window, GLFW_KEY_SPACE)) {
        camera_move(player->camera, PLAYER_UP, player->speed * state.dt);
    }

    if (window_is_key_pressed(state.window, GLFW_KEY_LEFT_SHIFT)) {
        camera_move(player->camera, PLAYER_UP, -player->speed * state.dt);
    }

    camera_rotate(player->camera, state.window->dx, state.window->dy);

    camera_update(player->camera, (float)state.window->width / (float)state.window->height);

    if (window_is_mouse_pressed(state.window, GLFW_MOUSE_BUTTON_LEFT) && player->click_couldown <= 0.0f) {
        ivec3 point;
        vec3 normal;
        if (is_picking_block(point, normal)) {
            world_remove_block(state.world, point);
        }
        player->click_couldown = 0.2f;
    }

    if (window_is_mouse_pressed(state.window, GLFW_MOUSE_BUTTON_RIGHT) && player->click_couldown <= 0.0f) {
        ivec3 point, new_pos;
        vec3 normal;
        if (is_picking_block(point, normal)) {
            calulate_new_block(point, normal, new_pos);
            world_add_block(state.world, new_pos, 1);
        }
        player->click_couldown = 0.2f;
    }

    player->click_couldown ? player->click_couldown -= state.dt : 0.0f;

    player->chunk_position[0] = div_floor(player->camera->position[0], CHUNK_SIZE);
    player->chunk_position[1] = 0;
    player->chunk_position[2] = div_floor(player->camera->position[2], CHUNK_SIZE);
}