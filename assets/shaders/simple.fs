#version 300 es
precision mediump float;
in vec3 position;
out vec4 outColor;
uniform float time;
uniform vec2 resolution;

void main() {
    outColor = vec4(1.0, 0.1, 0.1, 1.0);
}
