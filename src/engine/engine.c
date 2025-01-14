#include <windows.h>
#include <GL/gl.h>
#include <string.h>
#include <stdio.h>
#include "engine.h"
#include "sprite.h"
#include "renderer.h"
#include "camera.h"

static double get_time_seconds(void) {
    static LARGE_INTEGER frequency;
    static int initialized = 0;
    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = 1;
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
}

int engine_init(Engine* engine, int width, int height, const char* title) {
    if (!engine) return 0;

    engine->spriteCount = 0;

    if (!init_win32_window(&engine->window, width, height, title)) {
        return 0;
    }
    init_renderer(width, height);
    init_input_state(&engine->input);
    engine->running = 1;

    camera_init(&engine->camera, 0.0f, 0.0f, 1.0f);

    engine->width = width;
    engine->height = height;

    return 1;
}

int engine_add_sprite(Engine* engine, const Sprite* sprite) {
    if (!engine || !sprite) return -1;
    if (engine->spriteCount >= MAX_SPRITES) return -1;
    engine->sprites[engine->spriteCount] = *sprite;
    return engine->spriteCount++;
}

int engine_remove_sprite(Engine* engine, int index) {
    if (!engine) return 0;
    if (index < 0 || index >= engine->spriteCount) return 0;

    destroy_sprite(&engine->sprites[index]);

    for (int i = index; i < engine->spriteCount - 1; i++) {
        engine->sprites[i] = engine->sprites[i + 1];
    }

    engine->spriteCount--;
    return 1;
}

int engine_run(Engine* engine) {
    if (!engine) return 0;

    double lastTime = get_time_seconds();

    while (engine->running) {
        if (!process_win32_messages(&engine->window)) {
            engine->running = 0;
            break;
        }

        update_input_state(&engine->input);

        double currentTime = get_time_seconds();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        engine_update(engine, deltaTime);
        engine_render(engine);
    }

    return 1;
}

void engine_update(Engine* engine, float deltaTime) {
    if (!engine) return;

    float speed = 200.0f * deltaTime;
    if (is_key_down(&engine->input, 'W')) engine->camera.y += speed;
    if (is_key_down(&engine->input, 'D')) engine->camera.x += speed;
    if (is_key_down(&engine->input, 'S')) engine->camera.y -= speed;
    if (is_key_down(&engine->input, 'A')) engine->camera.x -= speed;
    
    if (is_key_down(&engine->input, 'Q')) camera_set_zoom(&engine->camera, engine->camera.zoom + 0.5f * deltaTime);
    if (is_key_down(&engine->input, 'E')) camera_set_zoom(&engine->camera, engine->camera.zoom - 0.5f * deltaTime);

    for (int i = 0; i < engine->spriteCount; i++) {
        update_sprite(&engine->sprites[i], deltaTime);
    }
}

void engine_render(Engine* engine) {
    if (!engine) return;

    begin_frame();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera_apply(&engine->camera, (float)engine->width, (float)engine->height);

    for (int i = 0; i < engine->spriteCount; i++) {
        draw_sprite_instance(&engine->sprites[i]);
    }

    end_frame();
}

void engine_set_camera_pos(Engine* engine, float x, float y) {
    if (!engine) return;
    camera_set_position(&engine->camera, x, y);
}

void engine_set_camera_zoom(Engine* engine, float zoom) {
    if (!engine) return;
    camera_set_zoom(&engine->camera, zoom);
}

void engine_move_camera(Engine* engine, float dx, float dy) {
    if (!engine) return;
    camera_set_position(&engine->camera, engine->camera.x + dx, engine->camera.y + dy);
}

void engine_zoom_camera(Engine* engine, float dZoom) {
    if (!engine) return;
    camera_set_zoom(&engine->camera, engine->camera.zoom + dZoom);
}

const InputState* engine_get_input_state(const Engine* engine) {
    if (!engine) return NULL;
    return &engine->input;
}

Sprite* engine_get_sprite(Engine* engine, int index) {
    if (!engine) return NULL;
    if (index < 0 || index >= engine->spriteCount) return NULL;
    return &engine->sprites[index];
}

int engine_get_sprite_count(const Engine* engine) {
    if (!engine) return 0;
    return engine->spriteCount;
}

void engine_shutdown(Engine* engine) {
    if (!engine) return;
    destroy_win32_window(&engine->window);
}
