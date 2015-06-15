#include <common/stepApp.h>

class TriangleApp: public byhj::Application
{
public:
	TriangleApp() 
	{
		windowInfo.title += "ch03-Triangle";
	}
	~TriangleApp() {}

	void v_Init()
	{
		init_buffer();
		//set the background color 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	void v_Render()
	{
		//clear the color buffer to backgroud color
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(1.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
private:
	void init_buffer();

	GLuint vbo;
};
CALL_MAIN(TriangleApp);

static const GLfloat VertexData[] = 
{
	-0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f,
	0.0f,  0.5f, 0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

void TriangleApp::init_buffer()
{
	//vbo are buffers that can be stored in video memory and provide the shortest access time to the GPU
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);

	//Hint the opengl how to send the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

