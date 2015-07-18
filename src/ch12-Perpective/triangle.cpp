#include "Triangle.h"

#include <glm/glm.hpp>
#include <glfw/glfw3.h>

#include "ogl/oglUtility.h"

namespace byhj
{

void Triangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Triangle::Render(const byhj::MvpMatrix &matrix)
{
	glUseProgram(program);
	glBindVertexArray(vao);

    float time = static_cast<float>( glfwGetTime() );


	static glm::mat4 mone = glm::mat4(1.0f);
	
	glm::mat4 world = glm::translate(mone, glm::vec3(0.0f, 0.0f, -5.0f) )
		            * glm::rotate(mone, time, glm::vec3(0.0f, 1.0f, 0.0f) );
	glm::mat4 proj = matrix.proj;
	glUniformMatrix4fv(0, 1, GL_FALSE, &(proj * world)[0][0] );

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

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
	-1.0f, -1.0f,  0.5773f,
	0.0f, -1.0f, -1.15475f,
	1.0f, -1.0f,  0.5773f,
	0.0f,  1.0f,  0.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

static const GLuint IndexData[] = 
{
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
};
static const GLsizei IndexSize = sizeof(IndexData);

void Triangle::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Triangle::init_vertexArray()
{
	//glBindBuffer will change the vao status
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

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