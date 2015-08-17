#ifndef SKYBOX_H
#define SKYBOX_H

#include "ogl/oglShader.h"
#include "ogl/model.h"

namespace byhj
{

class Skybox
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
	GLuint mvp_loc;
	GLuint tex_loc;
	GLuint cubemap_texture;
	OGLShader SkyboxShader = { "Skybox Shader" };
};


}
#endif