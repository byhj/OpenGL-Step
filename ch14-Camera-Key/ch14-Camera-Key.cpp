#include <GL/glew.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "byhj.h"

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


class Triangle_app : public byhj::Application 
{
public:
	Triangle_app():program(0), vao(0), vbo(0), ibo(0), 
		           TriangleShader("Triangle Shader") 
	{
		cameraPos = glm::vec3(0.0f, 0.0, 1.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -4.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	~Triangle_app(){}

public:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_unfiorm();

	virtual void initWindowInfo()
	{
		windowInfo.title = "CH2-Point";
		windowInfo.Width = 1300;
		windowInfo.Height = 900;
	}

	virtual void init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
	}

	virtual void render()
	{
		//Clear the color buffer
		static const GLfloat red[] = {0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, red);

		static float scale = 0.0f;
		scale += 0.01f;  //先平移，缩放，旋转
		
		//set model matrix
		glm::mat4 model = glm::mat4(1.0f); //注意矩阵顺序，先旋转后平移，如果先平移的话会产生不正常的现象！
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::rotate(model, scale, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);

		//set view matrix
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
		
		//set proj matrix
		glm::mat4 proj_matrix = glm::perspective(45.0f, windowInfo.aspect, 1.0f, 100.0f);
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj_matrix[0][0]);

		//render the scene
		glBindVertexArray(vao);
		glUseProgram(program);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
	}

	//clear the status
	virtual void shutdown()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

	// Is called whenever a key is pressed/released via GLFW
     void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	 if (key >= 0 && key < 1024 && action == GLFW_PRESS)
	 {
         GLfloat cameraSpeed = 10.0f * deltaTime;
        if (key == GLFW_KEY_W)
          cameraPos += cameraSpeed * cameraFront;
        if (key == GLFW_KEY_S)
          cameraPos -= cameraSpeed * cameraFront;
        if (key == GLFW_KEY_A)
          cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (key == GLFW_KEY_D)
          cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      }
   }
  
private:
	GLuint  program;
	Shader  TriangleShader;
	GLuint  vbo, vao, ibo;;
	GLuint  model_loc, view_loc, proj_loc;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
};

DECLARE_MAIN(Triangle_app);

void Triangle_app::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	TriangleShader.use();
	program = TriangleShader.program;
	model_loc = glGetUniformLocation(program, "model");
	view_loc = glGetUniformLocation(program, "view");
	proj_loc = glGetUniformLocation(program, "proj");
}

void Triangle_app::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);  //将索引数据传入
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle_app::init_vertexArray()
{

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);   //bind the vbo to vao, send the data to shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);
}
