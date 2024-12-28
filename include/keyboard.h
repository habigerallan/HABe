#ifndef KEYBOARD_H
#define KEYBOARD_H

void InitKeyboard();

void UpdateKeyboard();

int IsKeyPressed(int keyCode);

int DidKeyJustPress(int keyCode);

int DidKeyJustRelease(int keyCode);

#endif // KEYBOARD_H
