#version 430 core

layout (location = 0) out vec4 fragColor;

uniform sampler2D shadow;

in vec2 tc;

void main(void)
{
   float depth = texture(shadow, tc).x;
   depth = 1.0f - (1.0f - depth) * 25.0f;
   fragColor = vec4(depth);
}  