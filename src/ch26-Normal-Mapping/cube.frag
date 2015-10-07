#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT
{
  vec2 tc;
  vec3 normal;
  vec3 FragPos;
  vec3 tangent;
  vec3 biTangent;
}fs_in;

uniform sampler2D brickTex;
uniform sampler2D normalTex;

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

uniform Material mat;
uniform Light light;
uniform vec3 lightPos = vec3(0.5f, 0.5f, 1.0f);
uniform vec3 viewPos = vec3(0.0f, 0.0f, 3.0f);

void main(void)
{

   vs_out.tangent = normalize(vs_out.tangent);
   vs_out.biTangent = normalize(vs_out.biTangent);
   vs_out.normal = normalize(vs_out.normal);
   mat3 TBN = mat3(vs_out.tangent, vs_out.biTangent, vs_out.normal);
   vec3 normalMap = texture(normalTex, fs_in.tc).xyz;
   normalMap = normalize(2.0f * normalMap - 1.0f);
   normalMap *= TBN;

   vec4 texColor = texture(brickTex, fs_in.tc);

//Notice, you should normalize the light about vector
   vec3 lightDir   = normalize(lightPos - fs_in.FragPos);
   vec3 viewDir    = normalize(viewPos - fs_in.FragPos);
   vec3 reflectDir = reflect(-lightDir, normalMap);

//The phong light model = ambient + diffuse + specular
   vec3 ambient  = light.ambient * mat.ambient;
   vec3 diffuse  = light.diffuse * mat.diffuse * max(0.0f, dot(lightDir, normalMap) ); 
   vec3 specular = light.specular * mat.specular * pow( max( dot(reflectDir, viewDir), 0.0f ), mat.shininess);
   vec3 result   = ambient + diffuse;

   fragColor =  vec4(result, 1.0f);
}