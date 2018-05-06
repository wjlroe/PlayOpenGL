#ifndef _PLAY_OPENGL_MATHS_H_
#define _PLAY_OPENGL_MATHS_H_

#include <stdio.h>

#define TAU 2.0 * M_PI
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0  // 0.017444444
#define ONE_RAD_IN_DEG 360.0 / (2.0 * M_PI)  // 57.2957795

struct vec3 {
    float x;
    float y;
    float z;
    vec3();
    vec3(float x, float y, float z);
    void print() const;
};

struct vec4 {
    float x;
    float y;
    float z;
    float w;
    vec4();
    vec4(float x, float y, float z, float w);
    vec4 operator*(const float rhs);
    vec4 operator+(const vec4 &rhs);
    void print() const;
};

struct mat4 {
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
    mat4();
    mat4(vec4 x, vec4 y, vec4 z, vec4 w);
    mat4 operator*(const mat4 &rhs);
    vec4 operator*(const vec4 &rhs);
    void print() const;
    void into_array(float (&arr)[16]);
};

vec4 zero_vec4();
mat4 identity_mat4();
mat4 zero_mat4();
mat4 translate(const mat4 &a, const vec3 &v);
mat4 rotate_y_deg(const mat4 &a, const float angle);

#endif
