#version 430 core

out vec4 fragColor;
in vec2 texCoord0;

struct ambientLight {
  vec3 color;
  float intensity;
};
uniform ambientLight ambient;
uniform sampler2D gSampler; // texture data

void main(void)
 {
      fragColor = texture2D(gSampler, texCoord0.xy)
	              * vec4(ambient.color, 1.0f) * ambient.instensity;
 }