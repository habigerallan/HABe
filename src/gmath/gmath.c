#include <math.h>
#include "gmath.h"

Vec2 vec2_add(const Vec2 a, const Vec2 b) {
    Vec2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

Vec2 vec2_sub(const Vec2 a, const Vec2 b) {
    Vec2 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

Vec2 vec2_scale(const Vec2 v, float s) {
    Vec2 r;
    r.x = v.x * s;
    r.y = v.y * s;
    return r;
}

float vec2_dot(const Vec2 a, const Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float vec2_length(const Vec2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vec2 vec2_normalize(const Vec2 v) {
    float len = vec2_length(v);
    if (len == 0.0f) {
        Vec2 r = {0.0f, 0.0f};
        return r;
    }
    Vec2 r;
    r.x = v.x / len;
    r.y = v.y / len;
    return r;
}

Vec3 vec3_add(const Vec3 a, const Vec3 b) {
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

Vec3 vec3_sub(const Vec3 a, const Vec3 b) {
    Vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

Vec3 vec3_scale(const Vec3 v, float s) {
    Vec3 r;
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
    return r;
}

float vec3_dot(const Vec3 a, const Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(const Vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec3_normalize(const Vec3 v) {
    float len = vec3_length(v);
    if (len == 0.0f) {
        Vec3 r = {0.0f, 0.0f, 0.0f};
        return r;
    }
    Vec3 r;
    r.x = v.x / len;
    r.y = v.y / len;
    r.z = v.z / len;
    return r;
}

Mat4 mat4_identity(void) {
    Mat4 m;
    int i;
    for (i = 0; i < 16; i++) {
        m.m[i] = 0.0f;
    }
    m.m[0]  = 1.0f;
    m.m[5]  = 1.0f;
    m.m[10] = 1.0f;
    m.m[15] = 1.0f;
    return m;
}

Mat4 mat4_mul(const Mat4 A, const Mat4 B) {
    Mat4 R;
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            float sum = 0.0f;
            int i;
            for (i = 0; i < 4; i++) {
                sum += A.m[row + i*4] * B.m[i + col*4];
            }
            R.m[row + col*4] = sum;
        }
    }
    return R;
}

Mat4 mat4_translate(float x, float y, float z) {
    Mat4 m = mat4_identity();
    m.m[12] = x;
    m.m[13] = y;
    m.m[14] = z;
    return m;
}

Mat4 mat4_scale(float sx, float sy, float sz) {
    Mat4 m = mat4_identity();
    m.m[0]  = sx;
    m.m[5]  = sy;
    m.m[10] = sz;
    return m;
}

Mat4 mat4_rotate_z(float angleRadians) {
    Mat4 m = mat4_identity();
    float c = cosf(angleRadians);
    float s = sinf(angleRadians);
    m.m[0] =  c;
    m.m[1] =  s;
    m.m[4] = -s;
    m.m[5] =  c;
    return m;
}

Vec3 transform_vec2(const Vec2 v, float zLayer, const Mat4 transform) {
    float x = v.x;
    float y = v.y;
    float z = zLayer;
    float w = 1.0f;
    Vec3 r;
    r.x = transform.m[0]  * x + transform.m[4]  * y + transform.m[8]  * z + transform.m[12] * w;
    r.y = transform.m[1]  * x + transform.m[5]  * y + transform.m[9]  * z + transform.m[13] * w;
    r.z = transform.m[2]  * x + transform.m[6]  * y + transform.m[10] * z + transform.m[14] * w;
    return r;
}

Vec3 transform_vec3(const Vec3 v, const Mat4 transform) {
    float x = v.x;
    float y = v.y;
    float z = v.z;
    float w = 1.0f;
    Vec3 r;
    r.x = transform.m[0]  * x + transform.m[4]  * y + transform.m[8]  * z + transform.m[12] * w;
    r.y = transform.m[1]  * x + transform.m[5]  * y + transform.m[9]  * z + transform.m[13] * w;
    r.z = transform.m[2]  * x + transform.m[6]  * y + transform.m[10] * z + transform.m[14] * w;
    return r;
}
