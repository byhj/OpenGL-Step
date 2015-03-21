#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#include <shader.h>

GLuint vbo, vao, translate_loc, program;
Shader translateShader("translate shader");

glm::vec3 VertexData[3] = {
         glm::vec3( -1.0f, -1.0f, 0.0f ),
		 glm::vec3(  1.0f, -1.0f, 0.0f ),
		 glm::vec3(  0.0f,  1.0f, 0.0f ),
};

void load_shader()
{
	translateShader.init();
	translateShader.attach(GL_VERTEX_SHADER, "translate.vert");
	translateShader.attach(GL_FRAGMENT_SHADER, "translate.frag");
	translateShader.link();
    translateShader.use();
	program = translateShader.program;
	translate_loc = glGetUniformLocation(program, "translate_matrix");
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
	static float scale = 0.0f;
	scale += 0.01f;

	static float translate[4][4];
    translate[0][0] = 1.0f; translate[0][1] = 0.0f; translate[0][2] = 0.0f; translate[0][3] = sinf(scale);
    translate[1][0] = 0.0f; translate[1][1] = 1.0f; translate[1][2] = 0.0f; translate[1][3] = 0.0f;
    translate[2][0] = 0.0f; translate[2][1] = 0.0f; translate[2][2] = 1.0f; translate[2][3] = 0.0f;
    translate[3][0] = 0.0f; translate[3][1] = 0.0f; translate[3][2] = 0.0f; translate[3][3] = 1.0f;
	
	static const float black[] = {0.0f, 0.0f, 0.0f ,1.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	glBindVertexArray(vao);
	glViewport(0, 0, 720, 640);

	//glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(sinf(scale), 0.0f, 0.0f));
	//glUniformMatrix4fv(translate_loc, 1, GL_FALSE, &translate_matrix[0][0]);
	glUniformMatrix4fv(translate_loc, 1, GL_TRUE, &translate[0][0]); //行优先
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
	glutCreateWindow("Tutorial 6 - Translation");
	GLenum res = glewInit();
	if (res != GLEW_OK)
		cout << "Fail to initial the glew:" << glewGetString(res) << endl;
	init();
	glutDisplayFunc(render);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}