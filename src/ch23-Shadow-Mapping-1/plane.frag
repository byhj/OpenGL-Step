#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT
{
  vec2 tc;
}vs_out;

uniform sampler2D shadowTex;

void main(void)
{
   vec4 texColor = texture2D(shadowTex, vs_out.tc);

   fragColor = texColor;
}