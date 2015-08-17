#version 430 core

const int MAX_POINT_LIGHTS = 2;

layout (location = 0) out vec4 fragColor;

/////////////////////Input Block/////////////////////////////////
in VS_OUT
{
  vec2 tc;
  vec3 normal;
  vec3 fragPos;
}fs_in;


uniform sampler2D texture_diffuse1;
uniform vec3 LightDir = vec3(0.0f, 1.0f, -1.0f);
uniform vec3 viewPos = vec3(0.0f, 0.0f, 3.0f);

void main(void)
{

   vec3 normal   = normalize(fs_in.normal);
   vec3 viewDir  = normalize(viewPos - fs_in.fragPos);

   fragColor =  vec4(1.0f) * texture( texture_diffuse1, fs_in.tc);
}
