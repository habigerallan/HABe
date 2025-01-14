#include <windows.h>
#include <string.h>
#include "win32_window.h"
#include "input.h"

void init_input_state(InputState* input) {
    if (!input) return;
    memset(input->keys, KEY_RELEASED, sizeof(input->keys));
    memset(input->mouseButtons, KEY_RELEASED, sizeof(input->mouseButtons));
    input->mouseX = 0;
    input->mouseY = 0;
    input->mouseWheelDelta = 0;
}

void update_input_state(InputState* input) {
    if (!input) return;

    for (int i = 0; i < MAX_KEYS; i++) {
        short state = GetAsyncKeyState(i);
        if (state & 0x8000) {
            input->keys[i] = KEY_PRESSED;
        } else {
            input->keys[i] = KEY_RELEASED;
        }
    }

    // Left
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        input->mouseButtons[MOUSE_LEFT] = KEY_PRESSED;
    } else {
        input->mouseButtons[MOUSE_LEFT] = KEY_RELEASED;
    }

    // Right
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
        input->mouseButtons[MOUSE_RIGHT] = KEY_PRESSED;
    } else {
        input->mouseButtons[MOUSE_RIGHT] = KEY_RELEASED;
    }

    // Middle
    if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
        input->mouseButtons[MOUSE_MIDDLE] = KEY_PRESSED;
    } else {
        input->mouseButtons[MOUSE_MIDDLE] = KEY_RELEASED;
    }

    POINT p;
    if (GetCursorPos(&p)) {
        input->mouseX = p.x;
        input->mouseY = p.y;
    }

    input->mouseWheelDelta = get_mouse_wheel_delta();
}

int is_key_down(const InputState* input, unsigned char key) {
    if (!input) return 0;
    return (input->keys[key] == KEY_PRESSED) ? 1 : 0;
}

int is_mouse_button_down(const InputState* input, MouseButton button) {
    if (!input) return 0;
    if (button >= MAX_BUTTONS) return 0;
    return (input->mouseButtons[button] == KEY_PRESSED) ? 1 : 0;
}
