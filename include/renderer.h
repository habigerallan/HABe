#ifndef RENDERER_H
#define RENDERER_H

#include <windows.h>
#include <gl/gl.h>

#define STB_IMAGE_IMPLEMENTATION

int InitRenderer(HWND hwnd);
void DestroyRenderer();

unsigned int LoadTexture(const char* filepath);
void DestroyTexture(unsigned int textureID);

void Render(float x, float y, float z, float width, float height, float rotation, const float color[4], unsigned int textureID);

#endif // RENDERER_H
