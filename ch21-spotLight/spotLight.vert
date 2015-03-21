#version 430  core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out VS_OUT {
    vec2 tc;
    vec3 normal;
    vec3 worldPos;
	vec4 color;
}vs_out;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

void main() 
{    
     vs_out.tc = texCoord;  //纹理坐标
     vs_out.normal = mat3(model_matrix) * normal;  //法线坐标
	 vs_out.worldPos = mat3(model_matrix) * position;  //顶点在世界坐标
     vs_out.color = vec4(clamp(position, 0.0, 1.0), 1.0);

	 mat4 mvp_matrix = proj_matirx * view_matrix * model_matrix;
     gl_Position = mvp_matrix * vec4(position, 1.0); 
};