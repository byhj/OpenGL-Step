#version 430  core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out VS_OUT {
  vec2 tc;
  vec4 color;
}vs_out;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() 
{    
     vs_out.color = vec4(clamp(position, 0.0, 1.0), 1.0);
	 vs_out.tc = texCoord;
	 mat4 mvp = proj * view * model;
     gl_Position = mvp * vec4(position, 1.0); 

}