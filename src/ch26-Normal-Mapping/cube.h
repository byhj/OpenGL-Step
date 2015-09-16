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
	void Render(const glm::mat4 &mvp);
	void Shutdown(); 

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	ogl::Model SphereModel;
	GLuint program;
	GLuint texture;

	GLuint model_loc;
	GLuint view_loc;
	GLuint proj_loc;
	GLuint tex_loc;
	ogl::Shader CubeShader = { "Cube Shader" };
};


}
#endif