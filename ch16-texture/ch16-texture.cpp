#include <GL/glew.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "byhj.h"

struct Vertex {
	glm::vec3 pos; //顶点坐标
	glm::vec2 tex; //纹理坐标
	Vertex(const glm::vec3 &p, const glm::vec2 &t)
		:pos(p), tex(t){}
};

Vertex VertexData[4] = { 
	Vertex(glm::vec3(-1.0f, -1.0f, 0.5773f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.0f, -1.0f, -1.15475f), glm::vec2(0.5f, 0.0f)),
	Vertex(glm::vec3(1.0f, -1.0f, 0.5773f),  glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.0f, 1.0f, 0.0f),      glm::vec2(0.5f, 1.0f))
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
		           TriangleShader("Triangle Shader") {}
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
	    //glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		texture = loadTexture("../media/texture/test.png", true);
		init_shader();
		init_buffer();
		init_vertexArray();
	}

	virtual void render()
	{
		//Clear the color buffer
		static const GLfloat red[] = {0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat one[] = {1.0f};
		glClearBufferfv(GL_COLOR, 0, red);
		glClearBufferfv(GL_DEPTH, 0, one);
		static float scale = 0.0f;
		scale += 0.01f;  //先平移，缩放，旋转
		
		//set model matrix
		glm::mat4 model = glm::mat4(1.0f); //注意矩阵顺序，先旋转后平移，如果先平移的话会产生不正常的现象！
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, -60.0f, glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);

		//set view matrix
        glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
		
		//set proj matrix
		glm::mat4 proj_matrix = glm::perspective(camera.Zoom, windowInfo.aspect, 1.0f, 100.0f);
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj_matrix[0][0]);

		//render the scene
		glBindVertexArray(vao);
		glUseProgram(program);
		glBindTexture(GL_TEXTURE_2D, texture);
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


private:
	GLuint  program;
	Shader  TriangleShader;
	GLuint  vbo, vao, ibo;;
	GLuint  model_loc, view_loc, proj_loc;
    GLuint  texture, tex_loc;
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
	tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(tex_loc, 0);
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
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL); 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		                  (GLvoid*)( sizeof(glm::vec3) + NULL ) );
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);
}
