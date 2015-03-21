#version 430  core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

out vec4 color;

void main() 
{    
     normal0 = mat3(model_matrix) * normal;
	 worldPos0 = mat3(model_matrix) * position;
     color = vec4(clamp(position, 0.0, 1.0), 1.0);
	 mat4 mvp_matrix = proj_matirx * view_matrix * model_matrix;
     gl_Position = mvp_matrix * vec4(position, 1.0); 
	 texCoord0 = texCoord;
};