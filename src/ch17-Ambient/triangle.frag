#version 430 core

layout (location = 0) out vec4 fragColor;

in vec2 tc;
uniform sampler2D tex;

 struct Material 
{
	vec3 ambient;
};

struct Light 
{
   vec3 ambient;
};

uniform Material mat;
uniform Light light;

void main(void)
{
   vec3 ambient = light.ambient * mat.ambient;
   fragColor =  vec4(ambient, 1.0f) * texture(tex, tc);
}