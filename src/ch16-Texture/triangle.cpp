#include "Triangle.h"

#include <glm/glm.hpp>
#include <glfw/glfw3.h>

#include "ogl/Utility.h"
#include "ogl/Texture.h"

namespace byhj
{

void Triangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Triangle::Render(const ogl::MvpMatrix &matrix)
{
	glUseProgram(program);
	glBindVertexArray(vao);

	glm::mat4 view = matrix.view;
	glm::mat4 model = matrix.model;
	glm::mat4 proj = matrix.proj;

	glBindTexture(GL_TEXTURE_2D, tex);

	glUniformMatrix4fv(0, 1, GL_FALSE, &(proj * view * model)[0][0] );

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
	-1.0f, -1.0f,  0.5773f,	 0.0f, 0.0f,
	0.0f, -1.0f, -1.15475f,	 0.5f, 0.0f,
	1.0f, -1.0f,  0.5773f,	 1.0f, 0.0f,
	0.0f,  1.0f,  0.0f,		 0.5f, 1.0f,
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
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(NULL + 3 *sizeof(GLfloat)) );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindVertexArray(0);

}

void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	TriangleShader.use();
	TriangleShader.info();

	program = TriangleShader.GetProgram();
	mvp_loc = glGetUniformLocation(program, "mvp");
	tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(tex_loc, 0);
	assert(mvp_loc != INVALID_UNIFORM_LOCATION);
	assert(tex_loc != INVALID_UNIFORM_LOCATION);
}

void Triangle::init_texture()
{
	tex = loadTexture("../../media/texture/test.png");
}

}