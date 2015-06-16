#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent; 

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out VS_OUT
{
  vec2 tc;
  vec3 normal;
  vec3 FragPos;
  vec3 tangent;
}vs_out;

void main(void)
{
   mat4 mv = view * model;
   vs_out.tc = TexCoord;
   vs_out.normal = mat3(mv) * Normal;
   vs_out.FragPos = mat3(mv) * Position;
   Tangent0    = (model * vec4(Tangent, 0.0)).xyz;  
   mat4 mvp = proj * mv;

   gl_Position = mvp * vec4(Position, 1.0f);
}