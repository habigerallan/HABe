#include "keyboard.h"
#include <windows.h>

static int currentKeys[256];
static int previousKeys[256];

void InitKeyboard() {
    for (int i = 0; i < 256; i++) {
        currentKeys[i] = 0;
        previousKeys[i] = 0;
    }
}

void UpdateKeyboard() {
    for (int i = 0; i < 256; i++) {
        previousKeys[i] = currentKeys[i];
        currentKeys[i] = GetAsyncKeyState(i) & 0x8000 ? 1 : 0;
    }
}

int IsKeyPressed(int keyCode) {
    return currentKeys[keyCode];
}

int DidKeyJustPress(int keyCode) {
    return currentKeys[keyCode] && !previousKeys[keyCode];
}

int DidKeyJustRelease(int keyCode) {
    return !currentKeys[keyCode] && previousKeys[keyCode];
}
