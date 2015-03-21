#version 430  core

layout (location = 0) in vec3 position;

uniform float scale;

void main() 
{                  
     gl_Position = vec4(scale * position, 1.0);
};