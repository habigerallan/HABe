#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

int InitEngineWindow(const char* name, int width, int height, HINSTANCE hInstance);

void ProcessEngineWindowEvents();

int ShouldEngineWindowClosed();

void DestroyEngineWindow();

HWND GetEngineWindowHandle();

#endif // WINDOW_H
