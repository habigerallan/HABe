#include "HABeM.h"

HABeM_Vector2 HABeM_Vector2_add(HABeM_Vector2 a, HABeM_Vector2 b) {
    return (HABeM_Vector2){a.x + b.x, a.y + b.y};
}

HABeM_Vector2 HABeM_Vector2_sub(HABeM_Vector2 a, HABeM_Vector2 b) {
    return (HABeM_Vector2){a.x - b.x, a.y - b.y};
}
