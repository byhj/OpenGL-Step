#include "object.h"

namespace byhj
{


void Object::Init()
{
	init_buffer();
	init_shader();
}

void Object::Render(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj)
{
	glUseProgram(program);

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(view_loc,  1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(proj_loc,  1, GL_FALSE, &proj[0][0]);

	phoenixModel.Draw(program);

	glUseProgram(0);
}

void Object::Shutdown()
{
	glDeleteProgram(program);
}

void Object::init_buffer()
{
	phoenixModel.loadModel("../../media/phoenix_ugv.md2");
}

void Object::init_shader()
{
	ObjectShader.init();
	ObjectShader.attach(GL_VERTEX_SHADER, "object.vert");
	ObjectShader.attach(GL_FRAGMENT_SHADER, "object.frag");
	ObjectShader.link();
	program = ObjectShader.GetProgram();

	model_loc = glGetUniformLocation(program, "model");
	view_loc  = glGetUniformLocation(program, "view");
	proj_loc  = glGetUniformLocation(program, "proj");
}


}