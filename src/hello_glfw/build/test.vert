#version 410

in vec3 vp;
void main() {
  gl_Position = vec4(vp.x - 0.1, vp.y + 0.4, vp.z, 1.0);
};
