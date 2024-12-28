#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>
#include "sprite.h"

typedef struct {
    Sprite sprite;
} NeighborTile;

typedef struct {
    Sprite sprite;
    NeighborTile* neighbors[4];
} MainTile;

typedef void (*UpdateFunction)(Sprite* sprite, Sprite* allSprites, int spriteCount);

typedef struct {
    Sprite* sprites;
    int spritesLength;
    UpdateFunction* updateFunctions;
    MainTile* mainTiles;
    int gridwidth;
    int gridLength;
} Environment;

int InitEngine(const char* title, int width, int height, HINSTANCE hInstance);
void DestroyEngine();

void StartEngineLoop();

#endif // ENGINE_H
