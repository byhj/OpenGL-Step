#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#include <shader.h>

GLuint vbo, vao, scale_loc, program;
Shader interpShader("interp shader");

glm::vec3 VertexData[3] = {
         glm::vec3( -1.0f, -1.0f, 0.0f ),
		 glm::vec3(  1.0f, -1.0f, 0.0f ),
		 glm::vec3(  0.0f,  1.0f, 0.0f ),
};

void load_shader()
{
	interpShader.init();
	interpShader.attach(GL_VERTEX_SHADER, "interp.vert");
	interpShader.attach(GL_FRAGMENT_SHADER, "interp.frag");
	interpShader.link();
	interpShader.use();
	program = interpShader.program;
	scale_loc = glGetUniformLocation(program, "scale_matrix");
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
	interpShader.use();
	glBindVertexArray(vao);
	glViewport(0, 0, 720, 640);
	static float scale = 0.0f;
	scale += 0.01f;
	glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(sinf(scale), sinf(scale), 1.0));
	glUniformMatrix4fv(scale_loc, 1, GL_FALSE, &scale_matrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //显示模式，重要
	glutInitWindowPosition(150, 0);
	glutInitWindowSize(720, 640);
	glutCreateWindow("Tutorial 9 - interpolation");
	GLenum res = glewInit();
	if (res != GLEW_OK)
		cout << "Fail to initial the glew:" << glewGetString(res) << endl;
	init();
	glutDisplayFunc(render);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}