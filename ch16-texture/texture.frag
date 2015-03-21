#version 430 core

out vec4 fragColor;
in vec2 texCoord0;

uniform sampler2D gSampler; // texture data

void main(void)
 {
      fragColor = texture2D(gSampler, texCoord0.xy);
 }