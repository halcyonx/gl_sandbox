#type vertex
#version 300 es
layout(location = 0) in vec3 pos;
out vec3 position;

void main() {
    position = pos;
    gl_Position = vec4(pos, 1.0);
}

#type fragment
#version 300 es
precision mediump float;
in vec3 position;
out vec4 outColor;
uniform float time;
uniform vec2 resolution;

void main() {
    outColor = vec4(resolution.y/resolution.x, 0.0, 0.0, 1.0);
}
