#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 mvp;
uniform mat4 model;

out VS_OUT
{
  vec2 tc;
  vec3 normal;
  vec3 FragPos;
}vs_out;

void main(void)
{
   vs_out.tc = TexCoord;
   vs_out.normal = mat3(model) * Normal;
   vs_out.FragPos = mat3(model) * Position;
   gl_Position = mvp * vec4(Position, 1.0f);
}