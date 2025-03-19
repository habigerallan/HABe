#ifndef HABEM_H
#define HABEM_H

#include <math.h>

typedef struct {
    float x, y;
} HABeM_Vector2;

HABeM_Vector2 HABeM_Vector2_add(HABeM_Vector2 a, HABeM_Vector2 b);
HABeM_Vector2 HABeM_Vector2_sub(HABeM_Vector2 a, HABeM_Vector2 b);

#endif
