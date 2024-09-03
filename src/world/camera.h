#pragma once

#include <cglm/cglm.h>

#define CAMERA_YAW -90.0f
#define CAMERA_PITCH 0.0f

#define CAMERA_SPEED 2.5f
#define CAMERA_SENSITIVITY 0.1f

#define CAMERA_FOV 45.0f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 100.0f

#define CAMERA_POSITION (vec3){0.0f, 17.0f, 0.0f}
#define CAMERA_FRONT (vec3){0.0f, 0.0f, 1.0f}
#define CAMERA_UP (vec3){0.0f, 1.0f, 0.0f}
#define CAMERA_RIGHT (vec3){1.0f, 0.0f, 0.0f}

typedef struct camera_s {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;

    float yaw;
    float pitch;

    mat4 view;
    mat4 projection;
} camera_t;

camera_t *camera_create();
void camera_destroy(camera_t *camera);

void camera_update(camera_t *camera, float aspect);

void camera_move(camera_t *camera, vec3 direction, float delta);
void camera_rotate(camera_t *camera, float x_offset, float y_offset);