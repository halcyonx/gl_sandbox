#version 300 es

in vec2 TexCoord;
out vec4 color;

uniform sampler2D texture_0;

void main()
{
    color = texture(texture_0, TexCoord);
}
