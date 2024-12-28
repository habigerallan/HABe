#ifndef MOUSE_H
#define MOUSE_H

void InitMouse();

void UpdateMouse();

void GetMousePosition(int* x, int* y);

int IsMouseButtonPressed(int buttonCode);

int DidMouseButtonJustPress(int buttonCode);

int DidMouseButtonJustRelease(int buttonCode);

int GetMouseScrollDelta();

#endif // MOUSE_H
