#version 430  core

layout (location = 0) in vec4 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec4 vColor;

void main() 
{    
    //use position to output interpolation color
     vColor = clamp(position, 0.0, 1.0);    
	 mat4 mvp = proj * view * model;             
     gl_Position = mvp * position;
};