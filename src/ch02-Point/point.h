#ifndef Point_H
#define Point_H

#include <gl/glew.h>

namespace byhj
{

class Point
{
public:
	Point():vao(-1), vbo(-1) {}

	~Point() {}

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