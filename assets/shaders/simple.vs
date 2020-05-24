#version 300 es
layout(location = 0) in vec3 pos;
out vec3 position;

void main() {
    position = pos;
    gl_Position = vec4(pos, 1.0);
}
