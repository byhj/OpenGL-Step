#ifndef Cube_H
#define Cube_H

#include "ogl/Shader.h"
#include "ogl/model.h"

namespace byhj
{

class Cube
{
public:
	void Init();
	void Render(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj);
	void Shutdown(); 

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	ogl::Model SphereModel;
	GLuint program;
	GLuint brick_tex;
	GLuint normal_tex;

	GLuint model_loc;
	GLuint view_loc;
	GLuint proj_loc;
	GLuint brick_tex_loc;
	GLuint normal_tex_loc;

	ogl::Shader CubeShader = { "Cube Shader" };
};


}
#endif