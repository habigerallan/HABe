#ifndef GMATH_H
#define GMATH_H

typedef struct Vec2 {
    float x;
    float y;
} Vec2;

typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

typedef struct Mat4 {
    float m[16];
} Mat4;

Vec2 vec2_add(const Vec2 a, const Vec2 b);
Vec2 vec2_sub(const Vec2 a, const Vec2 b);
Vec2 vec2_scale(const Vec2 v, float s);
float vec2_dot(const Vec2 a, const Vec2 b);
float vec2_length(const Vec2 v);
Vec2 vec2_normalize(const Vec2 v);

Vec3 vec3_add(const Vec3 a, const Vec3 b);
Vec3 vec3_sub(const Vec3 a, const Vec3 b);
Vec3 vec3_scale(const Vec3 v, float s);
float vec3_dot(const Vec3 a, const Vec3 b);
float vec3_length(const Vec3 v);
Vec3 vec3_normalize(const Vec3 v);

Mat4 mat4_identity(void);
Mat4 mat4_mul(const Mat4 A, const Mat4 B);
Mat4 mat4_translate(float x, float y, float z);
Mat4 mat4_scale(float sx, float sy, float sz);
Mat4 mat4_rotate_z(float angleRadians);

Vec3 transform_vec2(const Vec2 v, float zLayer, const Mat4 transform);
Vec3 transform_vec3(const Vec3 v, const Mat4 transform);

#endif // GMATH_H