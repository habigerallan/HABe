#include "mouse.h"
#include <windows.h>

static int currentButtons[3];
static int previousButtons[3];
static int mouseX, mouseY;
static int mouseScrollDelta;

void InitMouse() {
    for (int i = 0; i < 3; i++) {
        currentButtons[i] = 0;
        previousButtons[i] = 0;
    }
    mouseX = mouseY = 0;
    mouseScrollDelta = 0;
}

void UpdateMouse() {
    previousButtons[0] = currentButtons[0];
    previousButtons[1] = currentButtons[1];
    previousButtons[2] = currentButtons[2];

    currentButtons[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
    currentButtons[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
    currentButtons[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(GetActiveWindow(), &cursorPos);
    mouseX = cursorPos.x;
    mouseY = cursorPos.y;

    mouseScrollDelta = 0;
}

void GetMousePosition(int* x, int* y) {
    *x = mouseX;
    *y = mouseY;
}

int IsMouseButtonPressed(int buttonCode) {
    return currentButtons[buttonCode];
}

int DidMouseButtonJustPress(int buttonCode) {
    return currentButtons[buttonCode] && !previousButtons[buttonCode];
}

int DidMouseButtonJustRelease(int buttonCode) {
    return !currentButtons[buttonCode] && previousButtons[buttonCode];
}

int GetMouseScrollDelta() {
    return mouseScrollDelta;
}
