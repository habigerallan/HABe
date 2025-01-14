#include <windows.h>
#include <GL/gl.h>
#include "camera.h"

void camera_init(Camera* cam, float x, float y, float zoom) {
    if (!cam) return;
    cam->x = x;
    cam->y = y;
    cam->zoom = zoom;
}

void camera_set_position(Camera* cam, float x, float y) {
    if (!cam) return;
    cam->x = x;
    cam->y = y;
}

void camera_set_zoom(Camera* cam, float zoom) {
    if (!cam) return;
    if (zoom <= 0.0f) zoom = 0.01f; 
    cam->zoom = zoom;
}

void camera_apply(const Camera* cam, float w, float h) {
    if (!cam) return;

    glTranslatef(w * 0.5f, h * 0.5f, 0.0f);
    glScalef(cam->zoom, cam->zoom, 1.0f);
    glTranslatef(-cam->x, -cam->y, 0.0f);
}
