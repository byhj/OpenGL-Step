#version 430 core

layout (location = 0) out vec4 fragColor;

/////////////////////Input Block/////////////////////////////////
in VS_OUT
{
  vec2 tc;
}fs_in;


uniform sampler2D texture_diffuse1;

void main(void)
{
   fragColor =  texture( texture_diffuse1, fs_in.tc);
}
