#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;

#include <shader.h>

GLuint vbo, vao;
Shader pointShader("point shader");
static const float VertexData[] = {0.0f, 0.0f, 0.0f ,0.0f};

void load_shader()
{
	pointShader.init();
	pointShader.attach(GL_VERTEX_SHADER, "point.vert");
	pointShader.attach(GL_FRAGMENT_SHADER, "point.frag");
	pointShader.link();
	pointShader.use();
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
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

void init()
{
	load_shader();
	init_buffer();
	init_vertexArray();
}

void render()
{
	static const float black[] = {0.0f, 0.0f, 0.0f ,1.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	pointShader.use();
	glBindVertexArray(vao);
	glViewport(0, 0, 720, 640);
	glPointSize(10.0f);
	glDrawArrays(GL_POINTS, 0, 1);
	glFlush();
}

int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA); //显示模式，重要
	glutInitWindowPosition(150, 0);
	glutInitWindowSize(720, 640);
	glutCreateWindow("Tutorial 2 - Dot");
	GLenum res = glewInit();
	if (res != GLEW_OK)
		cout << "Fail to initial the glew:" << glewGetString(res) << endl;
	init();
	glutDisplayFunc(render);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}