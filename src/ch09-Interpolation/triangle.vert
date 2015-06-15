#version 430 core

layout (location = 0) in vec4 Position;

uniform mat4 model;

out vec4 vColor;

void main(void)
{
   vColor = vec4(clamp(Position.xyz, 0.0f, 1.0f), 1.0f );
   gl_Position = model * Position;
}