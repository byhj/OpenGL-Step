#version 430 core

const vec4 VertexData[4] = {
                            vec4(-0.5f, -0.5f, 0.0f, 0.5f),
							vec4( 0.5f, -0.5f, 0.0f, 0.5f),
							vec4( 0.5f,  0.5f, 0.0f, 0.5f),
							vec4(-0.5f,  0.5f, 0.0f, 0.5f),
                           };

out vec2 tc;

void main(void)
{
   tc = VertexData[gl_VertexID].xy + vec2(0.5f);
   gl_Position = VertexData[gl_VertexID];
}