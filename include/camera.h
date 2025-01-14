#ifndef CAMERA_H
#define CAMERA_H

// centered
typedef struct {
    float x;
    float y;
    float zoom;
} Camera;

void camera_init(Camera* cam, float x, float y, float zoom);
void camera_set_position(Camera* cam, float x, float y);
void camera_set_zoom(Camera* cam, float zoom);
void camera_apply(const Camera* cam, float w, float h);

#endif
