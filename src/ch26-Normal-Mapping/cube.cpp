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

void Cube::Render(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj)
{
	glUseProgram(program);


	glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brick_tex);
	glUniform1i(brick_tex_loc, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_tex);
	glUniform1i(normal_tex_loc, 1);

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
	brick_tex_loc = glGetUniformLocation(program, "brickTex");
	normal_tex_loc = glGetUniformLocation(program, "normalTex");

	model_loc = glGetUniformLocation(program, "model");
	view_loc = glGetUniformLocation(program, "view");
	proj_loc = glGetUniformLocation(program, "proj");
}

void Cube::init_texture()
{
	brick_tex = ogl::loadTexture("../../media/textures/bricks.jpg");
	normal_tex = ogl::loadTexture("../../media/textures/normal_map.jpg");
}


}