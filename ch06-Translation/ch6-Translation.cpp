#include <GL/glew.h>
#include <shader.h>
#include <glm/glm.hpp>
#include "byhj.h"

glm::vec3 VertexData[3] = {
	glm::vec3( -1.0f, -1.0f, 0.0f ),
	glm::vec3(  1.0f, -1.0f, 0.0f ),
	glm::vec3(  0.0f,  1.0f, 0.0f ),
};

class Triangle_app : public byhj::Application 
{
public:
	Triangle_app():program(0), vao(0), vbo(0), ibo(0), 
		           TriangleShader("Triangle Shader") {}
	~Triangle_app(){}

public:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_unfiorm();

	virtual void initWindowInfo()
	{
		windowInfo.title = "CH2-Point";
		windowInfo.Width = 1300;
		windowInfo.Height = 900;
	}

	virtual void init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
	}

	virtual void render()
	{
		//Clear the color buffer
		static const GLfloat red[] = {0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, red);

		static float scale = 0.0f;
		static float translate[4][4];
		scale += 0.01f;
		translate[0][0] = 1.0f; translate[0][1] = 0.0f; translate[0][2] = 0.0f; translate[0][3] = sinf(scale);
		translate[1][0] = 0.0f; translate[1][1] = 1.0f; translate[1][2] = 0.0f; translate[1][3] = 0.0f;
		translate[2][0] = 0.0f; translate[2][1] = 0.0f; translate[2][2] = 1.0f; translate[2][3] = 0.0f;
		translate[3][0] = 0.0f; translate[3][1] = 0.0f; translate[3][2] = 0.0f; translate[3][3] = 1.0f;

	    glUniformMatrix4fv(model_loc, 1, GL_TRUE, &translate[0][0]); //––”≈œ»
		glBindVertexArray(vao);
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

	//clear the status
	virtual void shutdown()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

private:
	GLuint  program;
	Shader  TriangleShader;
	GLuint  vbo, vao, ibo;;
	GLuint  model_loc;
};

DECLARE_MAIN(Triangle_app);

void Triangle_app::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	TriangleShader.use();
	program = TriangleShader.program;
	model_loc = glGetUniformLocation(program, "model");
}

void Triangle_app::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle_app::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);   //bind the vbo to vao, send the data to shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}
