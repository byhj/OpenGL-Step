#version 430 core

layout (location = 0) in vec4 Position;

uniform float scale;

void main(void)
{
   gl_Position = vec4(scale, scale, 1.0f, 1.0f) * Position;
}