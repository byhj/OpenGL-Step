#version 430 core

out vec4 fragColor;
in vec2 texCoord0;
in vec3 normal0;

struct Light {
  vec3 color;
  float ambientIntensity;
  float directionIntensity;
  vec3 direction;
};

uniform Light light;
uniform sampler2D gSampler; // texture data

void main(void)
 {
      vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientInstensity;
      //…¢…‰π‚≤ø∑÷
	  float diffuseFactor = max(dot(normalize(normal0), -normalize(light.direction)), 0.0);
	  vec4 diffuseColor;
	  diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
	  fragColor = texture2D(gSampler, texCoord0.xy)
	              * (ambientColor + diffuseColor);
 }