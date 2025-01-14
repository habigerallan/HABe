#ifndef SPRITE_H
#define SPRITE_H

typedef enum {
    SPRITE_BEHAVIOR_NONE,
    SPRITE_BEHAVIOR_PLAYER_INPUT,
    SPRITE_BEHAVIOR_AI
} SpriteBehaviorType;

typedef struct Sprite Sprite;

typedef void (*SpriteUpdateFunc)(Sprite* sprite, float deltaTime);

typedef struct {
    SpriteBehaviorType type;
    SpriteUpdateFunc func;
} SpriteBehavior;

// bottom left
struct Sprite {
    float x;
    float y;
    float z;
    float width;
    float height;
    float rotation;
    unsigned int textureID;
    SpriteBehavior behavior;
};

Sprite create_sprite(
    float x,
    float y,
    float z,
    float width,
    float height,
    float rotation,
    const char* textureFile
);

void destroy_sprite(Sprite* s);
void set_sprite_position(Sprite* s, float x, float y, float z);
void set_sprite_rotation(Sprite* s, float rotation);
void set_sprite_size(Sprite* s, float width, float height);
void set_sprite_texture(Sprite* s, const char* textureFile);
void set_sprite_behavior(Sprite* s, SpriteBehaviorType type, SpriteUpdateFunc func);
void draw_sprite_instance(const Sprite* s);
void update_sprite(Sprite* s, float deltaTime);

#endif
