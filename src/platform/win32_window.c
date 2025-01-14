#include <windows.h>
#include "win32_window.h"

static int gWheelDeltaAccum = 0;

static LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_MOUSEWHEEL: 
            short delta = (short)HIWORD(wParam);
            gWheelDeltaAccum += delta;
            return 0;
        default:
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int init_win32_window(Win32Window* window, int width, int height, const char* title) {
    if (!window) return 0;

    window->hInstance = GetModuleHandle(NULL);
    window->width     = width;
    window->height    = height;
    window->title     = title;

    WNDCLASS wc       = {0};
    wc.style          = CS_OWNDC;
    wc.lpfnWndProc    = Win32WindowProc;
    wc.hInstance      = window->hInstance;
    wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName  = "Win32OpenGLWindowClass";

    if (!RegisterClass(&wc)) {
        return 0;
    }

    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    RECT rect     = {0, 0, width, height};
    AdjustWindowRect(&rect, dwStyle, FALSE);

    window->hwnd = CreateWindow(
        wc.lpszClassName,
        window->title,
        dwStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        window->hInstance,
        NULL
    );

    if (!window->hwnd) {
        return 0;
    }

    window->hdc = GetDC(window->hwnd);

    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize       = sizeof(pfd);
    pfd.nVersion    = 1;
    pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType  = PFD_TYPE_RGBA;
    pfd.cColorBits  = 24;
    pfd.cDepthBits  = 24;
    pfd.iLayerType  = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(window->hdc, &pfd);
    if (!pixelFormat) {
        return 0;
    }

    if (!SetPixelFormat(window->hdc, pixelFormat, &pfd)) {
        return 0;
    }

    window->hglrc = wglCreateContext(window->hdc);
    if (!window->hglrc) {
        return 0;
    }

    if (!wglMakeCurrent(window->hdc, window->hglrc)) {
        return 0;
    }

    ShowWindow(window->hwnd, SW_SHOW);
    UpdateWindow(window->hwnd);

    return 1;
}

int process_win32_messages(Win32Window* window) {
    (void)window;
    
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return 0; 
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 1;
}

void destroy_win32_window(Win32Window* window) {
    if (!window) return;

    wglMakeCurrent(NULL, NULL);
    if (window->hglrc) {
        wglDeleteContext(window->hglrc);
        window->hglrc = NULL;
    }
    if (window->hdc) {
        ReleaseDC(window->hwnd, window->hdc);
        window->hdc = NULL;
    }
    if (window->hwnd) {
        DestroyWindow(window->hwnd);
        window->hwnd = NULL;
    }
    UnregisterClass("Win32OpenGLWindowClass", window->hInstance);
}

int get_mouse_wheel_delta(void) {
    int delta = gWheelDeltaAccum / WHEEL_DELTA;
    gWheelDeltaAccum = 0;
    return delta;
}