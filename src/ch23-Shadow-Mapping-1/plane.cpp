#include "Plane.h"

namespace byhj
{


	void Plane::Init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();
	}

	void Plane::Render(GLuint shadowTex)
	{
		glUseProgram(program);
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowTex);
		glUniform1i(tex_loc, 0);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void Plane::Shutdown()
	{
		glDeleteProgram(program);
	}

	const static GLfloat VertexData[] =
	{
		-0.8f, -0.8f, 0.0f, 0.0f, 0.0f,
		 0.8f, -0.8f, 0.0f, 1.0f, 0.0f,
		 0.8f,  0.8f, 0.0f, 1.0f, 1.0f,
		-0.8f,  0.8f, 0.0f, 0.0f, 1.0f,
	};

	void Plane::init_buffer()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Plane::init_vertexArray()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(NULL + sizeof(GLfloat)* 3) );
		
		glBindVertexArray(0);
	}

	void Plane::init_shader()
	{
		PlaneShader.init();
		PlaneShader.attach(GL_VERTEX_SHADER, "Plane.vert");
		PlaneShader.attach(GL_FRAGMENT_SHADER, "Plane.frag");
		PlaneShader.link();
		program = PlaneShader.GetProgram();

		tex_loc = glGetUniformLocation(program, "shadowTex");
	}


}