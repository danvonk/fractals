#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec2 vUV;

void main() {
//     vCol = col;

     float x = float(((uint(gl_VertexID) + 2u) / 3u)%2u);
     float y = float(((uint(gl_VertexID) + 1u) / 3u)%2u);

     gl_Position = vec4(-1.0f + x*2.0f, -1.0f+y*2.0f, 0.0f, 1.0f);
     vUV = vec2(x, y);
}
