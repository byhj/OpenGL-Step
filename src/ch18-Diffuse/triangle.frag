#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT
{
  vec2 tc;
  vec3 normal;
  vec3 FragPos;
}fs_in;

uniform sampler2D tex;

 struct Material 
{
	vec4 ambient;
	vec4 diffuse;
};

struct Light 
{
   vec4 ambient;
   vec4 diffuse;
};

uniform Material mat;
uniform Light light;
uniform vec3 lightPos = vec3(0.5f, 0.5f, 1.0f);

void main(void)
{
   vec3 lightDir = (lightPos - fs_in.FragPos);
   vec3 normal = normalize(fs_in.normal);
   vec4 ambient = light.ambient * mat.ambient;
   vec4 diffuse = light.diffuse * mat.diffuse * max(0.0f, dot(lightDir, normal) ); 
   vec4 result = ambient + diffuse;

   fragColor =  result * texture(tex, fs_in.tc);
}