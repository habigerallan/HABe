#include "sprite.h"
#include "renderer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool InitSprite(Sprite* sprite, const char* filepath, float width, float height, float rotation) {
    if (!sprite) {
        return false;
    }

    if (filepath && strlen(filepath) > 0) {
        sprite->textureID = LoadTexture(filepath);
        if (sprite->textureID == 0) {
            printf("Failed to load texture: %s\n", filepath);
            return false;
        }
    } else {
        sprite->textureID = 0;
    }

    sprite->width = width;
    sprite->height = height;
    sprite->x = 0.0f;
    sprite->y = 0.0f;
    sprite->z = 0.0f;
    sprite->rotation = rotation;

    sprite->color[0] = 1.0f;
    sprite->color[1] = 1.0f;
    sprite->color[2] = 1.0f;
    sprite->color[3] = 1.0f;

    return true;
}

void DestroySprite(Sprite* sprite) {
    if (!sprite) {
        return;
    }

    if (sprite->textureID != 0) {
        DestroyTexture(sprite->textureID);
    }

    sprite->textureID = 0;
    sprite->width = 0;
    sprite->height = 0;
    sprite->x = 0.0f;
    sprite->y = 0.0f;
    sprite->rotation = 0.0f;

    sprite->color[0] = 1.0f;
    sprite->color[1] = 1.0f;
    sprite->color[2] = 1.0f;
    sprite->color[3] = 1.0f;
}

void SetSpritePosition(Sprite* sprite, float x, float y, float z) {
    if (!sprite) {
        return;
    }
    sprite->x = x;
    sprite->y = y;
    sprite->z = z;
}

void SetSpriteRotation(Sprite* sprite, float rotation) {
    if (!sprite) {
        return;
    }
    sprite->rotation = rotation;
}

void SetSpriteColor(Sprite* sprite, float r, float g, float b, float a) {
    if (!sprite) {
        return;
    }
    sprite->color[0] = r;
    sprite->color[1] = g;
    sprite->color[2] = b;
    sprite->color[3] = a;
}

void RenderSprite(const Sprite* sprite) {
    if (!sprite) {
        return;
    }

    Render(sprite->x, sprite->y, sprite->z, sprite->width, sprite->height, sprite->rotation, sprite->color, sprite->textureID);
}
