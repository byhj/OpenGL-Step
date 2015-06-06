#version 430 core

layout (location = 0) in vec3 Position;

uniform mat4 mvp;

out vec4 vColor;

void main(void)
{
   vColor = vec4(clamp(Position, 0.0f, 1.0f), 1.0f );
   gl_Position = mvp * vec4(Position, 1.0f);
}