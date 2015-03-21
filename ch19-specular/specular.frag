#version 430 core

out vec4 fragColor;
in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

struct Light {
  vec3 color;
  float ambientIntensity;
  float directionIntensity;
  vec3 direction;
};

uniform Light light;
uniform sampler2D gSampler; // texture data
uniform vec3 gEyeWorldPos;                                                          
uniform float gMatSpecularIntensity;                                                
uniform float gSpecularPower;

void main(void)
 {
      vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientInstensity;
      //散射光部分
	  float diffuseFactor = max(dot(normalize(normal0), -normalize(light.direction)), 0.0);
	  vec4 diffuseColor;
	  diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

	  vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0); //顶点到视点                     
      vec3 LightReflect = normalize(reflect(light.direction, Normal)); //反射光线
      float specularFactor = dot(VertexToEye, LightReflect);   //高光作用率
	  vec4 specularColor = max(vec4(light.color, 1.0f) * gMatSpecularIntensity * specularFactor, 0.0f);
	 
	  fragColor = texture2D(gSampler, texCoord0.xy)
	              * (ambientColor + diffuseColor + specularColor);
 }
