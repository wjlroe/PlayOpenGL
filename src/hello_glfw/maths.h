#ifndef _PLAY_OPENGL_MATHS_H_
#define _PLAY_OPENGL_MATHS_H_

#include <stdio.h>
// vec3, vec4
// mat4
// rotate_y_deg()

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
  void print() const;
};

vec4 zero_vec4();
mat4 identity_mat4();
mat4 zero_mat4();
mat4 translate(const mat4 &a, const vec3 &v);

#endif
