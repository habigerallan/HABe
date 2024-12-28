#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>

typedef struct {
    unsigned int textureID;
    float width;
    float height;
    float x;
    float y;
    float z;
    float rotation;
    float color[4];
} Sprite;

bool InitSprite(Sprite* sprite, const char* filepath, float width, float height, float rotation);
void DestroySprite(Sprite* sprite);

void SetSpritePosition(Sprite* sprite, float x, float y, float z);
void SetSpriteRotation(Sprite* sprite, float rotation);
void SetSpriteColor(Sprite* sprite, float r, float g, float b, float a);
void RenderSprite(const Sprite* sprite);

#endif // SPRITE_H
