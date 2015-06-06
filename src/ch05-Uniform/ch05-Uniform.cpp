#include <common/shader.h>
#include <common/common.h>
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
static Shader TriangleShader("Triangle Shader");
static GLuint g_vbo = 0, g_program = 0, g_vao = 0;
static GLuint g_scale_loc = -1;

static const GLfloat VertexData[] = 
{
	-1.0f,  -1.0f, 0.0f, 1.0f,
	 1.0f,  -1.0f, 0.0f, 1.0f,
	 0.0f,   1.0f, 0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

void init_buffer()
{
	//vbo are buffers that can be stored in video memory and provide the shortest access time to the GPU
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init_vertexArray()
{
	//vao is manage the opengl status
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	//We bind the buffer, change vao status
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);

	//Hint how opengl send the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);	
	
	//disable it before use
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	
}

void init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	g_program = TriangleShader.GetProgram();
	g_scale_loc = glGetUniformLocation(g_program, "scale");
	if (g_scale_loc == -1)
	{
		std::cerr << "Can not get the uniform location." << std::endl;
	}

}

void init()
{

#ifdef _DEBUG
	videoCardInfo();
#endif

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cerr << "Error:" << glewGetErrorString(res) << std::endl;
		return;
	}

	init_buffer();
	init_vertexArray();
	init_shader();

	//set the background color 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void render()
{
	//clear the color buffer to backgroud color
	glClear(GL_COLOR_BUFFER_BIT);

	//We use current shader program and vao status to render the scene
	glUseProgram(g_program);
	glBindVertexArray(g_vao);

	//We change the scale value every frame
	static float scale = 0.0f;
	scale += 0.001f;
	glUniform1f(g_scale_loc, sinf(scale) );

	glDrawArrays(GL_TRIANGLES, 0, 3);
	

	//Swap the buffer to show and make current window rediaplay
	glutSwapBuffers();
	glutPostRedisplay();
}

void shutdown()
{ 
	glDeleteProgram(g_program);
	glDeleteVertexArrays(1, &g_vao);
	glDeleteBuffers(1, &g_vbo);
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
	glutIdleFunc(render);
	glutMainLoop();
	shutdown();
}