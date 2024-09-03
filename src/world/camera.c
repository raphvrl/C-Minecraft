#include "camera.h"
#include "state.h"

camera_t *camera_create()
{
    camera_t *camera = malloc(sizeof(camera_t));
    if (camera == NULL) {
        return NULL;
    }

    glm_vec3_copy(CAMERA_POSITION, camera->position);
    glm_vec3_copy(CAMERA_FRONT, camera->front);
    glm_vec3_copy(CAMERA_UP, camera->up);
    glm_vec3_copy(CAMERA_RIGHT, camera->right);

    camera->yaw = CAMERA_YAW;
    camera->pitch = CAMERA_PITCH;

    glm_mat4_identity(camera->view);
    glm_mat4_identity(camera->projection);

    return camera;
}

void camera_destroy(camera_t *camera)
{
    if (camera == NULL) {
        return;
    }

    free(camera);
}

void camera_update(camera_t *camera, float aspect)
{
    glm_look(camera->position, camera->front, camera->up, camera->view);
    glm_perspective(CAMERA_FOV, aspect, CAMERA_NEAR, RENDER_DISTANCE * CHUNK_SIZE, camera->projection);
}

void camera_move(camera_t *camera, vec3 direction, float delta)
{
    float velocity = CAMERA_SPEED * delta;
    vec3 scaled_direction;
    glm_vec3_scale(direction, velocity, scaled_direction);
    glm_vec3_add(camera->position, scaled_direction, camera->position);
}

void camera_rotate(camera_t *camera, float x_offset, float y_offset)
{
    x_offset *= CAMERA_SENSITIVITY;
    y_offset *= CAMERA_SENSITIVITY;

    camera->yaw += x_offset;
    camera->pitch += y_offset;

    if (camera->pitch > 89.0f) {
        camera->pitch = 89.0f;
    }
    if (camera->pitch < -89.0f) {
        camera->pitch = -89.0f;
    }

    vec3 front;
    front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    front[1] = sin(glm_rad(camera->pitch));
    front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_vec3_normalize_to(front, camera->front);

    glm_vec3_crossn(camera->front, CAMERA_UP, camera->right);
    glm_vec3_crossn(camera->right, camera->front, camera->up);
}

