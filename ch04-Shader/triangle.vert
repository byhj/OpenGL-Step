#version 430  core

layout (location = 0) in vec4 position;

void main() 
{                  
     gl_Position = vec4(position.xy * 0.5, position.zw); 
};