#version 430 core

layout (location = 0) out vec4 fragColor;

in vec2 tc;
uniform sampler2D tex;

 struct Material 
{
	vec4 ambient;

};

struct Light 
{
   vec4 ambient;
};

uniform Material mat;
uniform Light light;

void main(void)
{
   vec4 ambient = light.ambient * mat.ambient;
   fragColor =  ambient * texture(tex, tc);
}