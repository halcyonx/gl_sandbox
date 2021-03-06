#version 300 es
in mediump vec2 TexCoord;

out mediump vec4 color;

uniform sampler2D texture_0;
uniform sampler2D texture_1;

void main()
{
    color = mix(texture(texture_0, TexCoord), texture(texture_1, TexCoord), 0.5);
}
