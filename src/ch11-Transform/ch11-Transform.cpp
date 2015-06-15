#include <common/stepApp.h>
#include <common/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TriangleApp: public byhj::Application
{
public:
	TriangleApp() :TriangleShader("Triangle Shader")
	{
		windowInfo.title += "11-Transform";
	}
	~TriangleApp() {}

	void v_Init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();

		//set the background color 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	void v_Render()
	{
		//clear the color buffer to backgroud color
		glClear(GL_COLOR_BUFFER_BIT);

		//We use current shader program and vao status to render the scene
		glUseProgram(program);
		glBindVertexArray(vao);

		static GLfloat time = 0.0f;
		time += 0.01f;
		static glm::mat4 mone = glm::mat4(1.0f);

		//You should add the translate to set last
		glm::mat4 world = glm::translate(mone, glm::vec3(sinf(time), 0.0f, 0.0f))
			* glm::rotate(mone, glm::radians(-sinf(time) * 90.0f), glm::vec3(0.0f, 0.0f, 1.0f) )
			* glm::rotate(mone, glm::radians(-sinf(time) * 90.0f), glm::vec3(0.0f, 1.0f, 0.0f) )
			* glm::rotate(mone, glm::radians(-sinf(time) * 90.0f), glm::vec3(1.0f, 0.0f, 0.0f) )
			* glm::scale(mone, glm::vec3( sinf(time * 0.1f), sinf(time * 0.1f), sinf(time * 0.1f) ) );

		//Notice the row-major or column-major 
		glUniformMatrix4fv(0, 1, GL_FALSE, &world[0][0]);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	}

	void v_Shutdown()
	{		
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

private:
	Shader TriangleShader;
	GLuint program;
	GLuint vbo, vao, ibo;
	GLuint model_loc;
};
CALL_MAIN(TriangleApp);

static const GLfloat VertexData[] = 
{
	-1.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 0.0f,
	0.0f,  1.0f, 0.0f,
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

void TriangleApp::init_buffer()
{
	//vbo are buffers that can be stored in video memory and provide the shortest access time to the GPU
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TriangleApp::init_vertexArray()
{
	//vao is manage the opengl status
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);
}

void TriangleApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
	model_loc = glGetUniformLocation(program, "model");
}

