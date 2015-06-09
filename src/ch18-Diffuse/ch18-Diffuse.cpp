#include <common/shader.h>
#include <common/common.h>
#include <common/camera.h>
#include <common/loadTexture.h>
#include <common/light.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stepApp.h"


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

class DiffuseApp: public byhj::StepApp
{
public:
	DiffuseApp(){};
	~DiffuseApp(){};

	void v_Init();
	void v_Render();
	void v_Shutdown();
	void v_Keyboard(unsigned char key, int x, int y);
	void v_MouseWheel(int wheel, int direction, int x, int y);
	void v_PassiveMouse(int xpos, int ypos);

	void init_buffer();
	void init_vertexArray();
	void init_texture();
	void init_shader();

private:
	GLuint program;
	GLuint vao, vbo, ibo;
	GLuint tex, tex_loc, mvp_loc;

	const char *WindowTitle;
	Shader TriangleShader;
	byhj::Light light;
	byhj::Material mat;
};

CALL_MAIN(DiffuseApp);

void  DiffuseApp::v_Init()
{

	//glEnable(GL_CULL_FACE);
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();

	//set the background color 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}


void DiffuseApp::v_Render()
{
	//clear the color buffer to backgroud color
	glClear(GL_COLOR_BUFFER_BIT);

	//We use current shader program and vao status to render the scene
	glUseProgram(program);
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, tex);

	static GLfloat time = 0.0f;
	time += 0.1f;
	static glm::mat4 mone = glm::mat4(1.0f);

	//You should add the translate to set last
	glm::mat4 world = glm::translate(mone, glm::vec3(0.0f, 0.0f, -2.0f) )
		* glm::rotate(mone, glm::radians(time), glm::vec3(0.0f, 1.0f, 0.0f) );
	static float currentTime = 0.0f;
	currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	deltaTime = currentTime - lastFrame;
	deltaTime *= 10.0f;
	lastFrame = currentTime;

	//glm::mat4 view = glm::lookAt(g_Camera.GetCamPos(), g_Camera.GetCamTarget(), g_Camera.GetCamUp());
	//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), g_Aspect, 1.0f, 100.0f);
	glm::mat4 mvp = proj * view * world;

	//Notice the row-major or column-major 
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0] );
	glUniform4fv(light.u_ambient_loc, 1, &light.ambient[0]);
	glUniform4fv(mat.u_ambient_loc, 1, &mat.ambient[0]);
	glUniform1i(tex_loc, 0);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	//Swap the buffer to show and make current window rediaplay
	glutSwapBuffers();
	glutPostRedisplay();
}

void DiffuseApp::v_Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}


static const GLfloat VertexData[] = 
{
	-1.0f, -1.0f,  0.5773f,	  0.0f, 0.0f,
	0.0f, -1.0f, -1.15475f,  0.5f, 0.0f,
	1.0f, -1.0f,  0.5773f,	  1.0f, 0.0f,
	0.0f,  1.0f,  0.0f,	  0.5f, 1.0f,
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

void DiffuseApp::init_buffer()
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

	light.ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mat.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
}

void DiffuseApp::init_vertexArray()
{
	//vao is manage the opengl status
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//We bind the buffer, change vao status
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Hint how opengl send the data
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(NULL + 3 * sizeof(GLfloat) ) );	

	//disable it before use
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

}

void DiffuseApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	TriangleShader.use();
	TriangleShader.interfaceInfo();
	program = TriangleShader.GetProgram();
	mvp_loc = glGetUniformLocation(program, "mvp");
	tex_loc = glGetUniformLocation(program, "tex");
	light.u_ambient_loc = glGetUniformLocation(program, "light.ambient");
	mat.u_ambient_loc = glGetUniformLocation(program, "mat.ambient");

}

void DiffuseApp::init_texture()
{
	tex = loadTexture("../../media/texture/test.png");
}

void DiffuseApp::v_Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key
		exit (0);
		break;
	case 'w':
		camera.ProcessKeyboard(FORWARD, deltaTime);
		break;
	case 's':
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		break;
	case 'd':
		camera.ProcessKeyboard(LEFT, deltaTime);
		break;
	case 'a':
		camera.ProcessKeyboard(RIGHT, deltaTime);
		break;
	case 'l':
		light.ambient += glm::vec4(0.1f);
		break;
	case 'k':
		light.ambient -= glm::vec4(0.1f);
		break;
	}

}

void DiffuseApp::v_MouseWheel(int wheel, int direction, int x, int y)
{
	camera.ProcessMouseScroll(direction);
}

void DiffuseApp::v_PassiveMouse(int xpos, int ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(-xoffset, -yoffset);
}

