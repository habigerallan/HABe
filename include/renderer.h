#ifndef RENDERER_H
#define RENDERER_H

#include "sprite.h"

void init_renderer(int width, int height);

void begin_frame(void);
void end_frame(void);

unsigned int load_texture(const char* filepath);
void free_texture(unsigned int textureID);

void draw_sprite(
    float x,
    float y,
    float z,
    float width,
    float height,
    float rotationRadians,
    unsigned int textureID
);

#endif
