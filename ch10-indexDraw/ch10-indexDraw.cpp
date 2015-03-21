#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#include <shader.h>

GLuint vbo, vao, ibo , rotate_loc, program;
Shader indexDrawShader("indexDraw shader");

const static glm::vec3 VertexData[4] = {
         glm::vec3(-1.0f, -1.0f, 0.0f ),
		 glm::vec3( 0.0f, -1.0f, 1.0f ),
		 glm::vec3( 1.0f, -1.0f, 0.0f ),
		 glm::vec3( 0.0f,  1.0f, 0.0f ),
};

const static GLushort ElementData[] = {
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
};

void load_shader()
{
	indexDrawShader.init();
	indexDrawShader.attach(GL_VERTEX_SHADER, "indexDraw.vert");
	indexDrawShader.attach(GL_FRAGMENT_SHADER, "indexDraw.frag");
	indexDrawShader.link();
	indexDrawShader.use();
	program = indexDrawShader.program;
	rotate_loc = glGetUniformLocation(program, "rotate_matrix");
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

void render()
{
	static const float black[] = {0.0f, 0.0f, 0.0f ,1.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	indexDrawShader.use();
	glBindVertexArray(vao);
	glViewport(0, 0, 720, 640);
	static float rotate = 0.0f;
	rotate += 0.01f;  //绕y轴旋转
	glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1.0f), rotate, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(rotate_loc, 1, GL_FALSE, &rotate_matrix[0][0]);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //显示模式，重要
	glutInitWindowPosition(150, 0);
	glutInitWindowSize(720, 640);
	glutCreateWindow("Tutorial 9 - indexDrawolation");
	GLenum res = glewInit();
	if (res != GLEW_OK)
		cout << "Fail to initial the glew:" << glewGetString(res) << endl;
	init();
	glutDisplayFunc(render);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}