#include <common/stepApp.h>
#include <common/shader.h>
#include <common/camera.h>
#include <common/Texture.h>
#include <common/light.h>
#include <common/common.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;

class TriangleApp: public byhj::Application
{
public:
	TriangleApp() :TriangleShader("Triangle Shader"), camera(glm::vec3(0.0f, 1.0f, 5.0f))
	{
		windowInfo.title += "16-Texture";
		lastX = GetScreenWidth() / 2.0f;
		lastY = GetScreenHeight() / 2.0f;
		firstMouse = true;
		deltaTime = 0.0f;
		lastTime = 0.0f;
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

		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		static GLfloat time = 0.0f;
		time += 1.0f;

		//You should add the translate to set last
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, -10.0f) );
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), GetAspect(), 1.0f, 100.0f);

		//Notice the row-major or column-major 
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj[0][0] );
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0] );
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
		glUniform3fv(viewPos_loc, 1, &camera.Position[0]);

		glUniform3fv(dirLight.ambient_loc, 1, &dirLight.ambient[0]);
		glUniform3fv(dirLight.diffuse_loc, 1, &dirLight.diffuse[0]);
		glUniform3fv(dirLight.specular_loc, 1, &dirLight.specular[0]);
		glUniform3fv(mat.ambient_loc, 1, &mat.ambient[0]);
		glUniform3fv(mat.diffuse_loc, 1, &mat.diffuse[0]);
		glUniform3fv(mat.specular_loc, 1, &mat.specular[0]);
		glUniform1f( mat.shininess_loc, mat.shininess);
		glUniform1i(tex_loc, 0);

		// Positions of the point lights
		glm::vec3 pointLightPositions[2] = 
		{
			glm::vec3(5.0f, 1.0f,  FieldDepth * cosf(currentTime)),
			glm::vec3(3.0f, 1.0f, FieldDepth * sinf(currentTime)),
		};

		glm::vec3 pointLightColors[2] = 
		{
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
		};

		// Point light 1
		glUniform3f(glGetUniformLocation(program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);		
		glUniform3f(glGetUniformLocation(program, "pointLights[0].ambient"), pointLightColors[0].x ,  pointLightColors[0].y ,  pointLightColors[0].z );		
		glUniform3f(glGetUniformLocation(program, "pointLights[0].diffuse"), pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z); 
		glUniform3f(glGetUniformLocation(program, "pointLights[0].specular"), pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
		glUniform1f(glGetUniformLocation(program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(program, "pointLights[0].linear"), 0.09);
		glUniform1f(glGetUniformLocation(program, "pointLights[0].quadratic"), 0.032);		

		// Point light 2				
		glUniform3f(glGetUniformLocation(program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);		
		glUniform3f(glGetUniformLocation(program, "pointLights[1].ambient"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);		
		glUniform3f(glGetUniformLocation(program, "pointLights[1].diffuse"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z); 
		glUniform3f(glGetUniformLocation(program, "pointLights[1].specular"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
		glUniform1f(glGetUniformLocation(program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(program, "pointLights[1].linear"), 0.09);
		glUniform1f(glGetUniformLocation(program, "pointLights[1].quadratic"), 0.032);	

		glUniform1i(tex_loc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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
	GLfloat lastTime;

	Shader TriangleShader;
	GLuint program;
	GLuint vbo, vao, ibo;
	GLuint mvp_loc, tex_loc, model_loc;
	GLuint proj_loc, view_loc, viewPos_loc;
	GLuint tex;

	enum PointLight
	{
		Size = 4
	};
	byhj::DirLight dirLight;
	byhj::PointLight pLight[PointLight::Size];
	byhj::Material mat;


};
CALL_MAIN(TriangleApp);

const glm::vec3 Normal = glm::vec3(0.0, 1.0f, 0.0f);
Vertex VertexData[6] = 
{
	Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), Normal),
	Vertex(glm::vec3(0.0f, 0.0f, FieldDepth), glm::vec2(0.0f, 1.0f), Normal),
	Vertex(glm::vec3(FieldWidth, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), Normal),

	Vertex(glm::vec3(FieldWidth, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), Normal),
	Vertex(glm::vec3(0.0f, 0.0f, FieldDepth), glm::vec2(0.0f, 1.0f), Normal),
	Vertex(glm::vec3(FieldWidth, 0.0f, FieldDepth), glm::vec2(1.0f, 1.0f), Normal)
};
static const GLsizei VertexSize = sizeof(VertexData);

void TriangleApp::init_buffer()
{
	//vbo are buffers that can be stored in video memory and provide the shortest access time to the GPU
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	dirLight.ambient  = glm::vec3(0.1f);
	dirLight.diffuse  = glm::vec3(0.5f);
	dirLight.specular = glm::vec3(1.0f);
	mat.ambient    = glm::vec3(0.1f);
	mat.diffuse    = glm::vec3(0.5f);
	mat.specular   = glm::vec3(1.0f);
	mat.shininess  = 32.0f;  
}



void TriangleApp::init_vertexArray()
{
	//vao is manage the opengl status
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
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
	program = TriangleShader.GetProgram();

	proj_loc           = glGetUniformLocation(program, "proj");  
	view_loc           = glGetUniformLocation(program, "view");  
	model_loc          = glGetUniformLocation(program, "model");
	tex_loc            = glGetUniformLocation(program, "tex");
	viewPos_loc        = glGetUniformLocation(program, "viewPos");

	dirLight.ambient_loc  = glGetUniformLocation(program, "dirLight.ambient");
	dirLight.diffuse_loc  = glGetUniformLocation(program, "dirLight.diffuse");
	dirLight.specular_loc = glGetUniformLocation(program, "dirLight.specular");
	dirLight.specular_loc = glGetUniformLocation(program, "dirLight.direction");

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
	if(keys[GLFW_KEY_L])
			dirLight.ambient += glm::vec3(0.1f);
	if(keys[GLFW_KEY_I])
			dirLight.ambient -= glm::vec3(0.1f);
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
