#version 410

uniform vec4 input_colour;
out vec4 frag_color;

void main() {
  frag_color = input_colour;
};
