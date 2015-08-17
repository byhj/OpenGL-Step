#ifndef Cube_H
#define Cube_H

#include "ogl/oglShader.h"
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

	Model SphereModel;
	GLuint program;
	GLuint model_loc;
	GLuint view_loc;
	GLuint proj_loc;
	GLuint tex_loc;
	GLuint texture;
	OGLShader CubeShader = { "Cube Shader" };
};


}
#endif