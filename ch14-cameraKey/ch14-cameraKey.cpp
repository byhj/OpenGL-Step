#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#include "shader.h"

GLuint vbo, vao, ibo , mv_matrix_loc, program;
GLuint proj_matrix_loc;
Shader cameraShader("camera shader");

const static glm::vec3 VertexData[4] = {
         glm::vec3(-1.0f, -1.0f, 0.5773f ),
		 glm::vec3( 0.0f, -1.0f, -1.15475f ),
		 glm::vec3( 1.0f, -1.0f, 0.5773f),
		 glm::vec3( 0.0f, 1.0f, 0.0f ),
};

const static GLushort ElementData[] = {
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
};

void load_shader()
{
	cameraShader.init();
	cameraShader.attach(GL_VERTEX_SHADER, "camera.vert");
	cameraShader.attach(GL_FRAGMENT_SHADER, "camera.frag");
	cameraShader.link();
	cameraShader.use();
	program = cameraShader.program;
	mv_matrix_loc = glGetUniformLocation(program, "mv_matrix");
	proj_matrix_loc = glGetUniformLocation(program, "proj_matrix");
}

void init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);  //将索引数据传入
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData),
		ElementData, GL_STATIC_DRAW);
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

static glm::vec3 target(0.0f, 0.0f, 0.0f);
static glm::vec3 pos(0.0f, 0.0f, -1.0f);
static glm::vec3 up(0.0f, 1.0f, 0.0f);

void render()
{
	static const float black[] = {0.0f, 0.0f, 0.0f ,1.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	cameraShader.use();
	glBindVertexArray(vao);
	glViewport(0, 0, 720, 640);
	static float scale = 0.0f;
	scale += 0.01f;  //先平移，缩放，旋转

	
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -3.0f))
                          *= glm::rotate(glm::mat4(1.0), scale, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 view_matrix = glm::lookAt(pos, target, up);
	glm::mat4 mv_matrix =  view_matrix * model_matrix ;
	glUniformMatrix4fv(mv_matrix_loc, 1, GL_FALSE, &mv_matrix[0][0]);

	glm::mat4 proj_matrix = glm::perspective(60.0f, 720.0f / 640.0f, 1.0f, 100.0f);
	glUniformMatrix4fv(proj_matrix_loc, 1, GL_FALSE, &proj_matrix[0][0]);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
	glutPostRedisplay();
}
static float stepScale = 0.1f;

void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: //变大
		pos += (target * stepScale);
		break;
	case GLUT_KEY_DOWN:  //变小
		pos -= (target * stepScale);;
		break;
	case GLUT_KEY_LEFT: {
		glm::vec3 left = glm::cross(target, up);
		glm::normalize(left);
		left *= stepScale;
		pos += left;
   }
		break;			
	case GLUT_KEY_RIGHT:{
		glm::vec3 right = glm::cross(up, target);
		glm::normalize(right);
		right *= stepScale;
		pos += right;
	}
		break;
	}
	glutPostRedisplay();
}
int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //显示模式，重要
	glutInitWindowPosition(150, 0);
	glutInitWindowSize(720, 640);
	glutCreateWindow("Tutorial 13 - camera");
	GLenum res = glewInit();
	if (res != GLEW_OK)
		cout << "Fail to initial the glew:" << glewGetString(res) << endl;
	init();
	glutDisplayFunc(render);
	glutMainLoop(); //循环调用注册函数display
	glutSpecialFunc(special);
	return 0;
}