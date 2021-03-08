#version 300 es

in mediump vec2 TexCoord;
out mediump vec4 color;

uniform sampler2D texture_0;

void main()
{
    color = texture(texture_0, TexCoord);
}
