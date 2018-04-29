#version 150 core

in vec3 vertex_position;
in vec3 vertex_colour;

layout(std140);
uniform mat4 view;
uniform mat4 proj;

out vec3 colour;

void main() {
    colour = vertex_colour;
    gl_Position = proj * view * vec4(vertex_position, 1.0);
};
