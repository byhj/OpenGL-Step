#include <common/stepApp.h>

class PointApp: public byhj::Application
{
public:
	PointApp() 
	{
		windowInfo.title += "ch02-Point";
	}
	~PointApp() {}

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

		//We set the point size is 10px to show result clearlly
		glPointSize(10.0f);
		glDrawArrays(GL_POINTS, 0, 1);
	}
private:
	void init_buffer();

	GLuint vbo;
};

CALL_MAIN(PointApp);


static const GLfloat VertexData[] = 
{
	0.0f, 0.0f, 0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

void PointApp::init_buffer()
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


