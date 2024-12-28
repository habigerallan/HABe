#include "window.h"

static HWND hwnd;
static int shouldClose = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            shouldClose = 1;
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int InitEngineWindow(const char* name, int width, int height, HINSTANCE hInstance) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MainWindowClass";

    if (!RegisterClass(&wc)) {
        return 0;
    }

    hwnd = CreateWindowEx(
        0,
        "MainWindowClass",
        name,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);
    return 1;
}

void ProcessEngineWindowEvents() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int ShouldEngineWindowClosed() {
    return shouldClose;
}

void DestroyEngineWindow() {
    DestroyWindow(hwnd);
}

HWND GetEngineWindowHandle() {
    return hwnd;
}
