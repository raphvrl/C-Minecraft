#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

texture_t *texture_create(const char *path)
{
    texture_t *texture = malloc(sizeof(texture_t));
    if (texture == NULL) {
        return NULL;
    }

    stbi_set_flip_vertically_on_load(true);
    stbi_uc *data = stbi_load(path, &texture->width, &texture->height, NULL, 4);
    if (data == NULL) {
        fprintf(stderr, "Failed to load texture: %s\n", path);
        free(texture);
        return NULL;
    }

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void texture_destroy(texture_t *texture)
{
    if (texture == NULL) {
        return;
    }

    glDeleteTextures(1, &texture->id);
    free(texture);
}

void texture_bind(texture_t *texture, GLuint slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void texture_unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
