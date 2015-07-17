#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>

namespace byhj
{

class Triangle
{
public:
	Triangle():vao(-1), vbo(-1) {}

	~Triangle() {}

public:
	void Init();
	void Render();
	void Shutdown();

private:

	void init_buffer();
	void init_vertexArray();

	GLuint vao, vbo;
};

}

#endif