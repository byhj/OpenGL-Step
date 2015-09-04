#include "Triangle.h"

#include <glm/glm.hpp>
#include <glfw/glfw3.h>

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

void Triangle::Render(const ogl::MvpMatrix &matrix, const ogl::Camera &camera)
{
	glUseProgram(program);
	glBindVertexArray(vao);

	glm::mat4 view = matrix.view;
	glm::mat4 model = matrix.model;
	glm::mat4 proj = matrix.proj;
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0] );
	glUniformMatrix4fv(view_loc, 1, GL_FALSE,  &view[0][0] );
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE,  &proj[0][0] );

	glm::vec3 camPos = camera.GetPos();
	glUniformMatrix3fv(viewPos_loc, 1, GL_FALSE, &camPos[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(tex_loc, 0);

    m_Light.SetUniform(program, glfwGetTime() );

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Triangle::Shutdown()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;
static const glm::vec3 Normal = glm::vec3(0.0, 1.0f, 0.0f);

static Vertex VertexData[6] = 
{
	Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), Normal),
	Vertex(glm::vec3(0.0f, 0.0f, FieldDepth), glm::vec2(0.0f, 1.0f), Normal),
	Vertex(glm::vec3(FieldWidth, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), Normal),

	Vertex(glm::vec3(FieldWidth, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), Normal),
	Vertex(glm::vec3(0.0f, 0.0f, FieldDepth), glm::vec2(0.0f, 1.0f), Normal),
	Vertex(glm::vec3(FieldWidth, 0.0f, FieldDepth), glm::vec2(1.0f, 1.0f), Normal)
};
static const GLsizei VertexSize = sizeof(VertexData);

void Triangle::init_buffer()
{

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_Light.Init();
}

void Triangle::init_vertexArray()
{
	//glBindBuffer will change the vao status
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)) );
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3) + sizeof(glm::vec2)) );

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	TriangleShader.info();
	program = TriangleShader.GetProgram();

	model_loc = glGetUniformLocation(program, "model");
	view_loc = glGetUniformLocation(program, "view");
	proj_loc = glGetUniformLocation(program, "proj");
	tex_loc = glGetUniformLocation(program, "tex");
	viewPos_loc   = glGetUniformLocation(program, "viewPos");

	assert(model_loc != INVALID_UNIFORM_LOCATION);
	assert(view_loc != INVALID_UNIFORM_LOCATION);
	assert(proj_loc != INVALID_UNIFORM_LOCATION);
	assert(tex_loc != INVALID_UNIFORM_LOCATION);

	m_Light.GetUniform(program);
}

void Triangle::init_texture()
{
	tex = loadTexture("../../media/texture/test.png");
}

void Triangle::AddAmbient()
{
	m_Light.AddAmbient();
}

void Triangle::MinusAmbient()
{
	m_Light.MinusAmbient();
}

}