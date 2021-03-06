#ifndef OBJECT_H
#define OBJECT_H

#include "ogl/Shader.h"
#include "ogl/model.h"

namespace byhj
{

class Object
{
 
public:
	void Init();
	void Render(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj);
	void Shutdown();

private:
	void init_buffer();
	void init_shader();

	GLuint program = 0;
	ogl::Model phoenixModel;
	GLuint model_loc;
	GLuint view_loc;
	GLuint proj_loc;
	ogl::Shader ObjectShader ={ "Object Shader" } ;
};


}
#endif