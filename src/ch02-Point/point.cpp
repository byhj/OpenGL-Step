#include "Point.h"

namespace byhj
{

void Point::Init()
{
	init_buffer();
	init_vertexArray();
}

void Point::Render()
{
	glBindVertexArray(vao);
	
	//Set the point occupy how many pixels
	glPointSize(30.0f);
	glDrawArrays(GL_POINTS, 0, 1);

	glBindVertexArray(0);
}

void Point::Shutdown()
{

}

static const GLfloat VertexData[] = 
{
	0.0f, 0.0f, 0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

void Point::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Point::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Hint the opengl how to send the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

}

}