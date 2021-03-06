#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"

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
	void Render(const ogl::MvpMatrix &matrix);
	void Shutdown();

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint tex;
    GLuint mvp_loc, tex_loc;
	GLuint vao, vbo, ibo;
	ogl::Shader TriangleShader;
	GLuint program;
};



}

#endif