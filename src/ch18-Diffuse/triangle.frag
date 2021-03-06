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
	vec3 ambient;
	vec3 diffuse;
};

struct Light 
{
   vec3 ambient;
   vec3 diffuse;
};

uniform Material g_Mat;
uniform Light g_Light;
uniform vec3 lightPos = vec3(0.5f, 0.5f, 1.0f);

void main(void)
{
   vec3 lightDir = normalize(lightPos - fs_in.FragPos);
   vec3 normal   = normalize(fs_in.normal);
   vec3 ambient  = g_Light.ambient * g_Mat.ambient;
   vec3 diffuse  = g_Light.diffuse * g_Mat.diffuse * max(0.0f, dot(lightDir, normal) ); 

   vec3 result   = ambient + diffuse;

   fragColor =  vec4(result, 1.0f) * texture(tex, fs_in.tc);
}