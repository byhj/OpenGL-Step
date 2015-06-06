#include <common/shader.h>
#include <common/common.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef WIN32
//If PLATFORM IS WIN32, we put the render window to the middle of window
const int g_ScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.75;
const int g_ScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.75;
const GLfloat g_Aspect = float(g_ScreenWidth) / float(g_ScreenHeight);
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
static GLuint g_vbo = 0, g_ibo = 0, g_program = 0, g_vao = 0;
static GLuint g_mvp_loc = -1;


glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

static GLfloat g_step = 0.0f;

static const GLfloat VertexData[] = 
{
	-1.0f, -1.0f,  0.5773f,
	 0.0f, -1.0f, -1.15475f,
	 1.0f, -1.0f,  0.5773f,
	 0.0f,  1.0f,  0.0f,
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

void init_buffer()
{
	//vbo are buffers that can be stored in video memory and provide the shortest access time to the GPU
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &g_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void init_vertexArray()
{
	//vao is manage the opengl status
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	//We bind the buffer, change vao status
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);

	//Hint how opengl send the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	
	
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
	g_mvp_loc = glGetUniformLocation(g_program, "mvp");
	if (g_mvp_loc == -1)
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

	static GLfloat time = 0.0f;
	time += 0.1f;
	static glm::mat4 mone = glm::mat4(1.0f);

	//You should add the translate to set last
	glm::mat4 world = glm::translate(mone, glm::vec3(0.0f, 0.0f, -5.0f) )
		* glm::rotate(mone, glm::radians(time), glm::vec3(0.0f, 1.0f, 0.0f) );
	static float currentTime = 0.0f;
	static float lastTime = 0.0f;
	static float elapsedTime = 0.0f;

	currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	elapsedTime = currentTime - lastTime;
	lastTime = currentTime;
	g_step = elapsedTime * 10.0f;

	//glm::mat4 view = glm::lookAt(g_Camera.GetCamPos(), g_Camera.GetCamTarget(), g_Camera.GetCamUp());
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 proj = glm::perspective(glm::radians(30.0f), g_Aspect, 1.0f, 100.0f);
	glm::mat4 mvp = proj * view * world;

					//Notice the row-major or column-major 
	glUniformMatrix4fv(0, 1, GL_FALSE, &mvp[0][0] );
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	//Swap the buffer to show and make current window rediaplay
	glutSwapBuffers();
	glutPostRedisplay();
}

void keyboard(int key, int x, int y)
{
	switch (key)
	{
	    case GLUT_KEY_UP:
			cameraPos += g_step * cameraFront;
		break;

		case GLUT_KEY_DOWN:
			cameraPos -= g_step * cameraFront;
		break;

		case GLUT_KEY_LEFT:
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * g_step;
	    break;

		case GLUT_KEY_RIGHT:
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * g_step;
	    break;
	}
	
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
	glutSpecialFunc(keyboard);
	glutIdleFunc(render);
	glutMainLoop();
	shutdown();
}