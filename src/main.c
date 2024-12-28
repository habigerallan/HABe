#include "engine.h"
#include "window.h"
#include "behavior.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    (void)hPrevInstance;
    (void)lpCmdLine; 
    
    if (!InitEngine("Engine", 800, 600, hInstance)) {
        return -1;
    }

    HWND hwnd = GetEngineWindowHandle();
    ShowWindow(hwnd, nShowCmd);

    StartEngineLoop();

    return 0;
}
