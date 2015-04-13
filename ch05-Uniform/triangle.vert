#version 430  core

layout (location = 0) in vec4 position;

uniform float scale;

void main() 
{                  
     gl_Position = vec4(position.xyz * scale, 1.0); 
};