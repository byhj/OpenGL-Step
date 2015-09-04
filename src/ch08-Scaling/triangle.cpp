#include "Triangle.h"

#include <glm/glm.hpp>
#include <glfw/glfw3.h>

#include "ogl/Utility.h"

namespace byhj
{

void Triangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Triangle::Render()
{
	glUseProgram(program);
	glBindVertexArray(vao);

    float time = static_cast<float>( glfwGetTime() );
	glm::mat4 world;

	world[0][0] = sinf(time);  
	world[1][1] = sinf(time); 
	world[2][2] = sinf(time); 

	//Notice the row-major or column-major 
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, &world[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Triangle::Shutdown()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

static const GLfloat VertexData[] = 
{
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f,  -1.0f, 0.0f, 1.0f,
	0.0f,   1.0f, 0.0f, 1.0f,
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

void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
	model_loc = glGetUniformLocation(program, "model");

	assert(model_loc != INVALID_UNIFORM_LOCATION);
}

}