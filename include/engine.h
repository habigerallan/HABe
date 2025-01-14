#ifndef ENGINE_H
#define ENGINE_H

#include "win32_window.h"
#include "input.h"
#include "sprite.h"
#include "camera.h"

#define MAX_SPRITES 32

typedef struct {
    Win32Window window;
    InputState input;
    int running;
    Sprite sprites[MAX_SPRITES];
    int spriteCount;
    Camera camera;
    int width;
    int height;
} Engine;

int engine_init(Engine* engine, int width, int height, const char* title);
int engine_run(Engine* engine);
void engine_shutdown(Engine* engine);

int engine_add_sprite(Engine* engine, const Sprite* sprite);
int engine_remove_sprite(Engine* engine, int index);

void engine_update(Engine* engine, float deltaTime);
void engine_render(Engine* engine);

void engine_set_camera_pos(Engine* engine, float x, float y);
void engine_set_camera_zoom(Engine* engine, float zoom);
void engine_move_camera(Engine* engine, float dx, float dy);
void engine_zoom_camera(Engine* engine, float dZoom);

const InputState* engine_get_input_state(const Engine* engine);
Sprite* engine_get_sprite(Engine* engine, int index);
int engine_get_sprite_count(const Engine* engine);

#endif
