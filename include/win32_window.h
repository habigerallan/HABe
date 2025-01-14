#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H

#include <windows.h>

typedef struct Win32Window {
    HINSTANCE hInstance;
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    int width;
    int height;
    const char* title;
} Win32Window;

int init_win32_window(Win32Window* window, int width, int height, const char* title);
int process_win32_messages(Win32Window* window);
void destroy_win32_window(Win32Window* window);

int get_mouse_wheel_delta(void);

#endif // WIN32_WINDOW_H