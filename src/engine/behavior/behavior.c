#include "behavior.h"
#include "keyboard.h"

void MoveSpriteWithWASD(Sprite* sprite, Sprite* allSprites, int spriteCount) {
    (void)allSprites;
    (void)spriteCount;

    const float speed = 0.1f;

    if (IsKeyPressed('W')) {
        sprite->y -= speed;
    }
    if (IsKeyPressed('A')) {
        sprite->x -= speed;
    }
    if (IsKeyPressed('S')) {
        sprite->y += speed;
    }
    if (IsKeyPressed('D')) {
        sprite->x += speed;
    }
}
