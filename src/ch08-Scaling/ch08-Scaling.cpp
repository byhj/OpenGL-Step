#include <common/stepApp.h>
#include <common/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TriangleApp: public byhj::Application
{
public:
	TriangleApp() :TriangleShader("Triangle Shader")
	{
		windowInfo.title += "ch08-Scaling";
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

		//We make the scale matrix
		static GLfloat time = 0.0f;
		time += 0.01f;
		glm::mat4 world;
		world[0][0] = sinf(time);  
		world[1][1] = sinf(time); 
		world[2][2] = sinf(time); 

		//Notice the row-major or column-major 
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, &world[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 3);
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
	GLuint vbo, vao;
	GLuint model_loc;
};
CALL_MAIN(TriangleApp);

static const GLfloat VertexData[] = 
{
	-1.0f,  -1.0f, 0.0f, 1.0f,
	 1.0f,  -1.0f, 0.0f, 1.0f,
	 0.0f,   1.0f, 0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

void TriangleApp::init_buffer()
{
	//vbo are buffers that can be stored in video memory and provide the shortest access time to the GPU
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleApp::init_vertexArray()
{
	//vao is manage the opengl status
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//We bind the buffer, change vao status
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Hint how opengl send the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);	

	//disable it before use
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

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

