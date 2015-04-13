#include <GL/glew.h>
#include <shader.h>
#include "byhj.h"

Shader PointShader("Point");

//Point data
const static GLfloat VertexData[] = 
{
	0.0f, 0.0f, 0.0f,
};

class test_app : public byhj::Application 
{
public:
	void init_shader()
	{
		PointShader.init();
		PointShader.attach(GL_VERTEX_SHADER, "point.vert");
		PointShader.attach(GL_FRAGMENT_SHADER, "point.frag");
		PointShader.link();
		PointShader.use();
		program = PointShader.program;
	}

	void init_buffer()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void init_vertexArray()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);   //bind the vbo to vao, send the data to shader
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

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

		glBindVertexArray(vao);
		glUseProgram(program);
		glPointSize(30.0f);
		glDrawArrays(GL_POINTS, 0, 1);
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
	GLuint  vbo, vao, ibo;;
};

DECLARE_MAIN(test_app);