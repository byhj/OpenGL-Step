#version 430 core

const int MAX_POINT_LIGHTS = 2;

layout (location = 0) out vec4 fragColor;

/////////////////////Input Block/////////////////////////////////
in VS_OUT
{
  vec2 tc;
  vec3 normal;
  vec3 fragPos;
}fs_in;

////////////////////Material///////////////////////////////////////
 struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

////////////////////Directional Light/////////////////////////////
struct DirLight 
{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

////////////////////Point Light/////////////////////////////////////
struct PointLight
{
   vec3 position;
   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};


//////////////////////Spot Light/////////////////////////////////
struct SpotLight
{
  vec3 direction;
  vec3 position;
  float cutOff;
  float outerCutOff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

////////////////////Uniform/////////////////////////////////////
uniform Material g_Mat;
uniform DirLight g_Light;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform sampler2D tex;
uniform vec3 LightDir = vec3(0.0f, 3.0f, 0.0f);
uniform vec3 viewPos = vec3(0.0f, 1.0f, 3.0f);

////////////////////////////Function//////////////////////////////////
vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pLight, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight sLight, vec3 normal, vec3 fragPos, vec3 viewDir);

///////////////////////////Main//////////////////////////////////////
void main(void)
{

   vec3 normal     = normalize(fs_in.normal);
   vec3 viewDir    = normalize(viewPos - fs_in.fragPos);

   //Phase 1:Calc the Diretional lighting
   vec3 result = CalcDirLight(g_Light, normal, viewDir);

   //Phase 2:Calc the Point Lighting
   for (int i = 0; i != MAX_POINT_LIGHTS; ++i)
     result += CalcPointLight(pointLights[i], normal, fs_in.fragPos, viewDir);

    // Phase 3: Spot light
   result += CalcSpotLight(spotLight, normal, fs_in.fragPos, viewDir);  

   fragColor =  vec4(result, 1.0f) * texture(tex, fs_in.tc);
}


//////////////////////////////Function define////////////////////////////////////
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir)
{
   
   vec3 lightDir   = normalize(LightDir);
   vec3 reflectDir = reflect(-lightDir, normal);

   //Ambient
   vec3 ambient  = light.ambient * g_Mat.ambient;
   //Diffuse
   float diff =  max(0.0f, dot(lightDir, normal) );
   vec3 diffuse  = diff * light.diffuse * g_Mat.diffuse ; 
   //Specular
   float spec =  pow( max( dot(reflectDir, viewDir), 0.0f ), g_Mat.shininess);
   vec3 specular = spec * light.specular * g_Mat.specular ;

   //Add all light 
   vec3 result   = ambient + diffuse + specular;

   return result;
}

//////////////////////////////Function define////////////////////////////////////
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
     // Attenuation
    float dis = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dis + light.quadratic * (dis * dis));    
  
   //Ambient
   vec3 ambient  = light.ambient * g_Mat.ambient;
   //Diffuse
   vec3 lightDir  = normalize(light.position - fragPos);
   float diff =  max(0.0f, dot(lightDir, normal) );
   vec3 diffuse  = diff * light.diffuse * g_Mat.diffuse ;
    
   //Specular
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec =  pow( max( dot(reflectDir, viewDir), 0.0f ), g_Mat.shininess);
   vec3 specular = spec * light.specular * g_Mat.specular ;

   //Add all light 
   vec3 result  = ambient + diffuse ; //+ specular;
   result *= attenuation;

	return result;
}

//////////////////////////////////////////////////////////////////////////////
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir  = normalize(light.position - fragPos);
  	// Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  // Attenuation
    float dis = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dis + light.quadratic * (dis * dis));    
  
   //Ambient
   vec3 ambient  = light.ambient * g_Mat.ambient;
   //Diffuse
   float diff =  max(0.0f, dot(lightDir, normal) );
   vec3 diffuse  = diff * light.diffuse * g_Mat.diffuse ;
   //Specular
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec =  pow( max( dot(reflectDir, viewDir), 0.0f ), g_Mat.shininess);
   vec3 specular = spec * light.specular * g_Mat.specular ;

   //Add all light 
   vec3 result  = ambient + diffuse ;//+ specular;
   result *= attenuation;

	return result * intensity;
}