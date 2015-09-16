#include "Cube.h"
#include "ogl/loadTexture.h"

namespace byhj
{

void Cube::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Cube::Render(const glm::mat4 &mvp)
{
	glUseProgram(program);


	glUniformMatrix4fv(model_loc, 1, GL_FALSE, &mvp[0][0]);

	SphereModel.Draw(program);

	glUseProgram(0);
}

void Cube::Shutdown()
{

}

void Cube::init_buffer()
{

	SphereModel.loadModel("../../media/cube.obj");

}

void Cube::init_vertexArray()
{

}

void Cube::init_shader()
{
	CubeShader.init();
	CubeShader.attach(GL_VERTEX_SHADER, "Cube.vert");
	CubeShader.attach(GL_FRAGMENT_SHADER, "Cube.frag");
	CubeShader.link();
	program = CubeShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "normalTex");
	model_loc = glGetUniformLocation(program, "model");
	view_loc = glGetUniformLocation(program, "view");
	proj_loc = glGetUniformLocation(program, "proj");
}

void Cube::init_texture()
{

}

}