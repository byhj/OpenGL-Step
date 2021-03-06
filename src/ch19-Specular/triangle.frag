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
	vec3 specular;
	float shininess;
};

struct Light 
{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform Material g_Mat;
uniform Light g_Light;
uniform vec3 lightPos = vec3(0.0f, 0.0f, 1.0f);
uniform vec3 viewPos = vec3(0.0f, 0.0f, 3.0f);

void main(void)
{

//Notice, you should normalize the g_Light about vector
   vec3 normal     = normalize(fs_in.normal);
   vec3 lightDir   = normalize(lightPos - fs_in.FragPos);
   vec3 viewDir    = normalize(viewPos - fs_in.FragPos);
   vec3 reflectDir = reflect(-lightDir, normal);

//The phong g_Light model = ambient + diffuse + specular
   vec3 ambient  = g_Light.ambient * g_Mat.ambient;
   vec3 diffuse  = g_Light.diffuse * g_Mat.diffuse * max(0.0f, dot(lightDir, normal) ); 
   vec3 specular = g_Light.specular * g_Mat.specular * pow( max( dot(reflectDir, viewDir), 0.0f ), g_Mat.shininess);
   vec3 result   = ambient + diffuse + specular;

   fragColor =  vec4(result, 1.0f) * texture(tex, fs_in.tc);
}