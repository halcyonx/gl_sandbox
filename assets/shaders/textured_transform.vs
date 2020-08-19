#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform mat4 transform;

void main()
{
    TexCoord = aTexCoord;
    gl_Position = transform * vec4(aPosition, 1.0f);
}