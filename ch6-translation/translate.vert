#version 430  core

layout (location = 0) in vec3 position;

uniform mat4 translate_matrix;

void main() 
{                  
     gl_Position = translate_matrix * vec4(position, 1.0);
};