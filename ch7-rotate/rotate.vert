#version 430  core

layout (location = 0) in vec3 position;

uniform mat4 rotate_matrix;

void main() 
{                  
     gl_Position = rotate_matrix * vec4(position, 1.0f);
};