#include "maths.h"

vec3::vec3() {}

vec3::vec3(float px, float py, float pz) {
  x = px;
  y = py;
  z = pz;
}

vec4::vec4() {}

vec4::vec4(float px, float py, float pz, float pw) {
  x = px;
  y = py;
  z = pz;
  w = pw;
}

vec4 vec4::operator*(const float rhs) {
  float nx = x * rhs;
  float ny = y * rhs;
  float nz = z * rhs;
  float nw = w * rhs;
  return vec4(nx, ny, nz, nw);
}

vec4 vec4::operator+(const vec4 &rhs) {
  float nx = x + rhs.x;
  float ny = y + rhs.y;
  float nz = z + rhs.z;
  float nw = w + rhs.w;
  return vec4(nx, ny, nz, nw);
}

mat4::mat4() {}

mat4::mat4(vec4 px, vec4 py, vec4 pz, vec4 pw) {
  x = px;
  y = py;
  z = pz;
  w = pw;
}

mat4 mat4::operator*(const mat4 &rhs) {
  vec4 nx = x * rhs.x.x + y * rhs.x.y + z * rhs.x.z + w * rhs.x.w;
  vec4 ny = x * rhs.y.x + y * rhs.y.y + z * rhs.y.z + w * rhs.y.w;
  vec4 nz = x * rhs.z.x + y * rhs.z.y + z * rhs.z.z + w * rhs.z.w;
  vec4 nw = x * rhs.w.x + y * rhs.w.y + z * rhs.w.z + w * rhs.w.w;
  return mat4(nx, ny, nz, nw);
}

vec4 zero_vec4() { return vec4(0.0f, 0.0f, 0.0f, 0.0f); }

mat4 identity_mat4() {
  return mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 0.0f),
              vec4(0.0f, 0.0f, 1.0f, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

mat4 zero_mat4() {
  return mat4(zero_vec4(), zero_vec4(), zero_vec4(), zero_vec4());
}

mat4 translate(const mat4 &a, const vec3 &v) {
  printf("translate mat4:");
  a.print();
  printf(" by vec3:");
  v.print();
  mat4 translated = identity_mat4();
  translated.x.w = v.x;
  translated.y.w = v.y;
  translated.z.w = v.z;
  return (translated * a);
}

void mat4::print() const {
  printf("\n");
  printf("[%.2f][%.2f][%.2f][%.2f]\n", x.x, x.y, x.z, x.w);
  printf("[%.2f][%.2f][%.2f][%.2f]\n", y.x, y.y, y.z, y.w);
  printf("[%.2f][%.2f][%.2f][%.2f]\n", z.x, z.y, z.z, z.w);
  printf("[%.2f][%.2f][%.2f][%.2f]\n", w.x, w.y, w.z, w.w);
}

void vec3::print() const {
  printf("\n");
  printf("[%.2f][%.2f][%.2f]\n", x, y, z);
}

void vec4::print() const {
  printf("\n");
  printf("[%.2f][%.2f][%.2f][%.2f]\n", x, y, z, w);
}
