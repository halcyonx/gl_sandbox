#version 300 es
precision mediump float;
in vec3 position;
out vec4 outColor;
uniform float time;
uniform vec2 resolution;

void main() {
    outColor = vec4(0.85, 0.2, 0.3, 1.0);
}
