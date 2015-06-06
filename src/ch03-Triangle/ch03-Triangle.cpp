#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#ifdef WIN32
//If PLATFORM IS WIN32, we put the render window to the middle of window
const int g_ScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.75;
const int g_ScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.75;
const int g_PosX = (GetSystemMetrics(SM_CXSCREEN) - g_ScreenWidth) / 2;
const int g_PosY = (GetSystemMetrics(SM_CYSCREEN) - g_ScreenHeight) / 2;

#else
const int g_ScreenWidth = 800;
const int g_ScreenHeight = 600;
const int g_PosX = 300;
const int g_PosY = 100;
#endif

//Window Title
const char *g_pWindowTitle = "ch3-Triangle";
static GLuint g_vbo = 0;

static const GLfloat VertexData[] = 
{
	-0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f,
	0.0f,  0.5f, 0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

void init_buffer()
{
	//vbo are buffers that can be stored in video memory and provide the shortest access time to the GPU
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);

	//Hint the opengl how to send the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init()
{
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cerr << "Error:" << glewGetErrorString(res) << std::endl;
		return;
	}

	init_buffer();

	//set the background color 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void render()
{
	//clear the color buffer to backgroud color
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//Swap the buffer to show
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_ScreenWidth, g_ScreenHeight);
	glutInitWindowPosition(g_PosX, g_PosY);
	glutCreateWindow(g_pWindowTitle);
	init();
	glutDisplayFunc(render);
	glutMainLoop();
}
