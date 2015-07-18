#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include "ogl/oglShader.h"

namespace byhj
{

class Triangle
{
public:
	Triangle():vao(-1), vbo(-1), program(-1) ,TriangleShader("Triangle Shader"){}

	~Triangle() {}

public:
	void Init();
	void Render();
	void Shutdown();

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();

	struct Uniform
	{
       GLuint scale_loc;
	}uniform;

	GLuint vao, vbo;
	OGLShader TriangleShader;
	GLuint program;
};



}

#endif