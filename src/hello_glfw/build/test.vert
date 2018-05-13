#version 150 core

in vec3 vertex_position;
in vec3 vertex_colour;
out vec3 colour;

layout(std140) uniform;
uniform mat4 view;
uniform mat4 proj;

void main() {
    colour = vertex_colour;
    gl_Position = proj * view * vec4(vertex_position, 1.0);
}
