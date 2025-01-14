#include <stdlib.h>
#include "sprite.h"
#include "renderer.h"

static void default_update(Sprite* sprite, float deltaTime) {
    (void)sprite;
    (void)deltaTime;
}

Sprite create_sprite(
    float x,
    float y,
    float z,
    float width,
    float height,
    float rotation,
    const char* textureFile
) {
    Sprite s;
    s.x = x;
    s.y = y;
    s.z = z;
    s.width = width;
    s.height = height;
    s.rotation = rotation;
    s.textureID = load_texture(textureFile);

    s.behavior.type = SPRITE_BEHAVIOR_NONE;
    s.behavior.func = default_update;

    return s;
}

void destroy_sprite(Sprite* s) {
    if (!s) return;
    if (s->textureID) {
        free_texture(s->textureID);
        s->textureID = 0;
    }
}

void set_sprite_position(Sprite* s, float x, float y, float z) {
    if (!s) return;
    s->x = x;
    s->y = y;
    s->z = z;
}

void set_sprite_rotation(Sprite* s, float rotation) {
    if (!s) return;
    s->rotation = rotation;
}

void set_sprite_size(Sprite* s, float width, float height) {
    if (!s) return;
    s->width = width;
    s->height = height;
}

void set_sprite_texture(Sprite* s, const char* textureFile) {
    if (!s) return;
    if (s->textureID) {
        free_texture(s->textureID);
    }
    s->textureID = load_texture(textureFile);
}

void set_sprite_behavior(Sprite* s, SpriteBehaviorType type, SpriteUpdateFunc func) {
    if (!s) return;
    s->behavior.type = type;
    s->behavior.func = func ? func : default_update;
}

void draw_sprite_instance(const Sprite* s) {
    if (!s) return;
    draw_sprite(s->x, s->y, s->z, s->width, s->height, s->rotation, s->textureID);
}

void update_sprite(Sprite* s, float deltaTime) {
    if (!s) return;
    if (s->behavior.func) {
        s->behavior.func(s, deltaTime);
    }
}