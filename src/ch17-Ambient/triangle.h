#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/light.h"

namespace byhj
{

class Triangle
{
public:
	Triangle():vao(-1), vbo(-1), ibo(-1),  program(-1), mvp_loc(-1), tex_loc(-1),
		       TriangleShader("Triangle Shader") {}

	~Triangle() {}

public:
	void Init();
	void Render(const byhj::MvpMatrix &matrix);
	void Shutdown();

	void AddAmbient();
	void MinusAmbient();

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint tex;
    GLuint mvp_loc, tex_loc;
	GLuint vao, vbo, ibo;
	OGLShader TriangleShader;
	GLuint program;

	byhj::Light m_Light;
	byhj::Material m_Mat;
};



}

#endif