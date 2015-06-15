#include <common/stepApp.h>
#include <common/shader.h>
#include <common/camera.h>
#include <common/Texture.h>
#include <common/light.h>
#include <common/common.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TriangleApp: public byhj::Application
{
public:
	TriangleApp() :TriangleShader("Triangle Shader")
	{
		windowInfo.title += "16-Texture";
		lastX = GetScreenWidth() / 2.0f;
		lastY = GetScreenHeight() / 2.0f;
		firstMouse = true;
		deltaTime = 0.0f;
		lastFrame = 0.0f;
		for (int i = 0; i != 1024; ++ i)
			keys[i] = false;
	}
	~TriangleApp() {}

	void v_Init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();
		init_texture();
		glEnable(GL_CULL_FACE);

		//set the background color 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	void v_Render()
	{
		//clear the color buffer to backgroud color
		glClear(GL_COLOR_BUFFER_BIT);

		//We use current shader program and vao status to render the scene
		glUseProgram(program);
		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D, tex);

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		static GLfloat time = 0.0f;
		time += 1.0f;
		static glm::mat4 mone = glm::mat4(1.0f);

		//You should add the translate to set last
		glm::mat4 model = glm::translate(mone, glm::vec3(0.0f, 0.0f, -5.0f) )
			* glm::rotate(mone, glm::radians(time), glm::vec3(0.0f, 1.0f, 0.0f) );

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), GetAspect(), 1.0f, 100.0f);
		glm::mat4 mvp = proj * view * model;

		//Light
		glUniform3fv(light.ambient_loc, 1, &light.ambient[0]);
		glUniform3fv(light.diffuse_loc, 1, &light.diffuse[0]);
		glUniform3fv(light.specular_loc, 1, &light.specular[0]);

		//Material
		glUniform3fv(mat.ambient_loc, 1, &mat.ambient[0]);
		glUniform3fv(mat.diffuse_loc, 1, &mat.diffuse[0]);
		glUniform3fv(mat.specular_loc, 1, &mat.specular[0]);
		glUniform1f(mat.shininess_loc, mat.shininess);

		glUniform1i(tex_loc, 0);

		//Notice the row-major or column-major 
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj[0][0] );
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0] );
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix3fv(viewPos_loc, 1, GL_FALSE, &camera.Position[0]);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	}

	void v_Shutdown()
	{		
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
	void v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void v_Movement(GLFWwindow *window);
	void v_MouseCallback(GLFWwindow* window, double xpos, double ypos);
	void v_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();
private:
	// Camera
	Camera camera;
	bool keys[1024];
	GLfloat lastX, lastY ;
	bool firstMouse;
	GLfloat deltaTime;
	GLfloat lastFrame;

	Shader TriangleShader;
	GLuint program;
	GLuint vbo, vao, ibo;
	GLuint mvp_loc, tex_loc, model_loc;
	GLuint tex;
	byhj::Light light;
	byhj::Material mat;
	GLuint proj_loc, view_loc, viewPos_loc;
};
CALL_MAIN(TriangleApp);

Vertex VertexData[4] =
{ 
	Vertex(glm::vec3(-1.0f, -1.0f, 0.5773f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.0f, -1.0f, -1.15475f), glm::vec2(0.5f, 0.0f)),
	Vertex(glm::vec3(1.0f, -1.0f, 0.5773f),  glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.0f, 1.0f, 0.0f),      glm::vec2(0.5f, 1.0f))
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

void TriangleApp::init_buffer()
{
	//We calc the vertex normal 
	unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(VertexData);
	unsigned int IndexCount = ARRAY_SIZE_IN_ELEMENTS(IndexData);
	CalcNormals(IndexData, IndexCount, VertexData, VertexCount);

	//vbo are buffers that can be stored in video memory and provide the shortest access time to the GPU
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	light.ambient  = glm::vec3(1.0f);
	light.diffuse  = glm::vec3(1.0f);
	light.specular = glm::vec3(1.0f);
	mat.ambient    = glm::vec3(0.1f);
	mat.diffuse    = glm::vec3(0.5f);
	mat.specular   = glm::vec3(1.0f);
	mat.shininess  = 16.0f;  
}



void TriangleApp::init_vertexArray()
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
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)) );
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3) + sizeof(glm::vec2)) );

	//disable it before use
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void TriangleApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	TriangleShader.use();
	program = TriangleShader.GetProgram();

	proj_loc           = glGetUniformLocation(program, "proj");  
	view_loc           = glGetUniformLocation(program, "view");  
	model_loc          = glGetUniformLocation(program, "model");
	tex_loc            = glGetUniformLocation(program, "tex");
	viewPos_loc        = glGetUniformLocation(program, "viewPos");
	light.ambient_loc  = glGetUniformLocation(program, "light.ambient");
	light.diffuse_loc  = glGetUniformLocation(program, "light.diffuse");
	light.specular_loc = glGetUniformLocation(program, "light.specular");
	mat.ambient_loc    = glGetUniformLocation(program, "mat.ambient");
	mat.diffuse_loc    = glGetUniformLocation(program, "mat.diffuse");
	mat.specular_loc   = glGetUniformLocation(program, "mat.specular");
	mat.shininess_loc  = glGetUniformLocation(program, "mat.shininess");


}


void TriangleApp::init_texture()
{
	tex = loadTexture("../../media/texture/test.png");
}


// Moves/alters the camera positions based on user input
void TriangleApp::v_Movement(GLFWwindow *window)
{
	// Camera controls
	if(keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if(keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if(keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if(keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_C])
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		camera.ctr = false;
	}
}

// Is called whenever a key is pressed/released via GLFW
void TriangleApp::v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << std::endl;
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if(action == GLFW_PRESS)
		keys[key] = true;
	else if(action == GLFW_RELEASE)
		keys[key] = false;	
}

void TriangleApp::v_MouseCallback(GLFWwindow* window, double xpos, double ypos)
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
	camera.ProcessMouseMovement(xoffset, yoffset);
}	


void TriangleApp::v_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

	camera.ProcessMouseScroll(yoffset);
}

