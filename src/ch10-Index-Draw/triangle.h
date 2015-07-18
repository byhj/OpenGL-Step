#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include "ogl/oglShader.h"

namespace byhj
{

class Triangle
{
public:
	Triangle():vao(-1), vbo(-1), ibo(-1), 
		       program(-1), model_loc(-1),
	           TriangleShader("Triangle Shader") {}

	~Triangle() {}

public:
	void Init();
	void Render();
	void Shutdown();

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();

    GLuint model_loc;
	GLuint vao, vbo, ibo;
	OGLShader TriangleShader;
	GLuint program;
};



}

#endif