#ifndef INPUT_H
#define INPUT_H

#define MAX_KEYS 256
#define MAX_BUTTONS 3

typedef enum KeyState {
    KEY_RELEASED = 0,
    KEY_PRESSED
} KeyState;

typedef enum MouseButton {
    MOUSE_LEFT   = 0,
    MOUSE_RIGHT  = 1,
    MOUSE_MIDDLE = 2
} MouseButton;

typedef struct InputState {
    KeyState keys[MAX_KEYS];
    KeyState mouseButtons[MAX_BUTTONS];
    int mouseX;
    int mouseY;
    int mouseWheelDelta;
} InputState;

void init_input_state(InputState* input);
void update_input_state(InputState* input);

int is_key_down(const InputState* input, unsigned char key);
int is_mouse_button_down(const InputState* input, MouseButton button);

#endif // INPUT_H