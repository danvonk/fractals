#version 450

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 col;

out vec3 vCol;

void main() {
     vCol = col;
     gl_Position = vec4(pos, 0.0, 1.0);
}
