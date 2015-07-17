#include "Triangle.h"

namespace byhj
{

void Triangle::Init()
{
	init_buffer();
	init_vertexArray();
}

void Triangle::Render()
{
	glBindVertexArray(vao);
	
	//Draw a red triangle
	glColor3f(1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
}

void Triangle::Shutdown()
{

}

static const GLfloat VertexData[] = 
{
	-0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f,
	0.0f,  0.5f, 0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

void Triangle::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

}

}