#version 450

in vec3 vCol;
out vec4 fCol;

void main() {
    fCol = vec4(vCol, 1.0);
}