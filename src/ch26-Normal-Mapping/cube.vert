#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent; 
layout (location = 4) in vec3 BiTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT
{
  vec2 tc;
  vec3 normal;
  vec3 FragPos;
  vec3 tangent;
  vec3 biTangent;
}vs_out;

void main(void)
{

   vs_out.tc = TexCoord;

   mat4 mv = view * model;
   vs_out.FragPos   = mat3(mv) * Position;
   vs_out.normal    = (model * vec4(Normal, 0.0)).xyz; 
   vs_out.tangent   = (model * vec4(Tangent, 0.0)).xyz;  
   vs_out.biTangent = (model * vec4(BiTangent, 0.0)).xyz;  

   mat4 mvp = proj * mv;

   gl_Position = mvp * vec4(Position, 1.0f);
}