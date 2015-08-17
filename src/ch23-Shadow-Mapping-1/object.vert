#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out VS_OUT
{
  vec2 tc;
}vs_out;

void main(void)
{
   vs_out.tc = TexCoord;
   mat4 mvp = proj * view * model;
   gl_Position = mvp * vec4(Position, 1.0f);
}