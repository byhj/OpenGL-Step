#version 430  core

layout (location = 0) in vec3 position;

uniform mat4 scale_matrix;
out vec4 color;

void main() 
{      //return min(max(x, minVal), maxVal)
     color = vec4(clamp(position, 0.0, 1.0), 1.0);       
     gl_Position = scale_matrix * vec4(position, 1.0);
};