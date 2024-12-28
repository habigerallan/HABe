#include "engine.h"
#include "window.h"
#include "renderer.h"
#include "keyboard.h"
#include "behavior.h"
#include "mouse.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

Environment* env = NULL;
static HDC hDC = NULL;

int InitEngine(const char* title, int width, int height, HINSTANCE hInstance) {
    if (!InitEngineWindow(title, width, height, hInstance)) {
        printf("Failed to create the window.\n");
        return 0;
    }

    hDC = GetDC(GetEngineWindowHandle());
    if (!hDC) {
        printf("Failed to get device context.\n");
        return 0;
    }

    if (!InitRenderer(GetEngineWindowHandle())) {
        printf("Failed to initialize the renderer.\n");
        return 0;
    }

    InitKeyboard();
    InitMouse();

    env = (Environment*)malloc(sizeof(Environment));
    if (!env) {
        printf("Failed to allocate memory for the environment.\n");
        return 0;
    }

    env->sprites = NULL;
    env->spritesLength = 0;
    env->updateFunctions = NULL;
    env->mainTiles = NULL;
    env->gridwidth = 0;
    env->gridLength = 0;

    env->spritesLength = 3;
    env->sprites = (Sprite*)malloc(sizeof(Sprite) * env->spritesLength);
    if (!env->sprites) {
        printf("Failed to allocate memory for sprites.\n");
        free(env);
        return 0;
    }

    env->updateFunctions = (UpdateFunction*)calloc(env->spritesLength, sizeof(UpdateFunction));
    if (!env->updateFunctions) {
        printf("Failed to allocate memory for update functions.\n");
        free(env->sprites);
        free(env);
        return 0;
    }

    InitSprite(&env->sprites[2], "", 100.0f, 100.0f, 30.0f);
    SetSpriteColor(&env->sprites[2], 1, 0, 0, 1);
    SetSpritePosition(&env->sprites[2], 500, 20, -1);

    InitSprite(&env->sprites[0], "", 100.0f, 100.0f, 0.0f);
    SetSpriteColor(&env->sprites[0], 1, 1, 0, 1);
    SetSpritePosition(&env->sprites[0], 300, 300, 1);

    InitSprite(&env->sprites[1], "C:\\Users\\habig\\Documents\\GitHub\\HABe\\assets\\test.png", 100.0f, 100.0f, 0.0f);

    env->updateFunctions[1] = MoveSpriteWithWASD;

    return 1;
}

void ProcessInput() {
    UpdateKeyboard();
    UpdateMouse();
}

void UpdateEnvironment() {
    if (!env || !env->sprites) return;

    for (int i = 0; i < env->spritesLength; ++i) {
        if (env->updateFunctions && env->updateFunctions[i]) {
            env->updateFunctions[i](&env->sprites[i], env->sprites, env->spritesLength);
        }
    }
}

void RenderEnvironment() {
    if (!env || !env->sprites) return;

    for (int i = 0; i < env->spritesLength; ++i) {
        RenderSprite(&env->sprites[i]);
    }
}

void StartEngineLoop() {
    while (!ShouldEngineWindowClosed()) {
        ProcessEngineWindowEvents();
        ProcessInput();

        UpdateEnvironment();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderEnvironment();

        SwapBuffers(hDC);
    }

    DestroyEngine();
}

void DestroyEngine() {
    if (env) {
        if (env->sprites) {
            for (int i = 0; i < env->spritesLength; ++i) {
                DestroySprite(&env->sprites[i]);
            }
            free(env->sprites);
        }

        if (env->updateFunctions) {
            free(env->updateFunctions);
        }

        if (env->mainTiles) {
            for (int i = 0; i < env->gridLength; ++i) {
                for (int j = 0; j < 4; ++j) {
                    if (env->mainTiles[i].neighbors[j]) {
                        DestroySprite(&env->mainTiles[i].neighbors[j]->sprite);
                        free(env->mainTiles[i].neighbors[j]);
                    }
                }
                DestroySprite(&env->mainTiles[i].sprite);
            }
            free(env->mainTiles);
        }

        free(env);
        env = NULL;
    }

    DestroyRenderer();

    if (hDC) {
        ReleaseDC(GetEngineWindowHandle(), hDC);
        hDC = NULL;
    }

    DestroyEngineWindow();
}