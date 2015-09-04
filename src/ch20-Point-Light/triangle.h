#ifndef Triangle_H
#define Triangle_H

#ifndef _DEBUG
#define _NDEBUG
#endif

#include <gl/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/camera.h"

#include "light.h"

namespace byhj
{

class Triangle
{
public:
	Triangle():vao(-1), vbo(-1), ibo(-1),  program(-1), 
		       model_loc(-1), view_loc(-1), proj_loc(-1), 
			   tex_loc(-1), TriangleShader("Triangle Shader") {}

	~Triangle() {}

public:
	void Init();
	void Render(const ogl::MvpMatrix &matrix, const ogl::Camera &camera);
	void Shutdown();

	void AddAmbient();
	void MinusAmbient();

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint tex;
    GLuint model_loc, view_loc, proj_loc;
	GLuint viewPos_loc, tex_loc;
	GLuint vao, vbo, ibo;
	ogl::Shader TriangleShader;
	GLuint program;
	byhj::Light m_Light;
};



}

#endif