#version 430 core

layout (location = 0) in vec4 Position;

uniform mat4 model;

void main(void)
{
   gl_Position = model * Position;
}