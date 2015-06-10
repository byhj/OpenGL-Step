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
#include <common/glDebug.h>
#include "common/stepApp.h"


Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
glm::vec3 cameraPos   = glm::vec3(0.0f, 1.0f,  5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

static const int PointLightSize = 2;

class DiffuseApp: public byhj::StepApp
{
public:
	DiffuseApp():AppShader("Diffuse Shader") {};
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
	GLuint tex, tex_loc, proj_loc,view_loc, model_loc;
	GLuint viewPos_loc;

	Shader AppShader;
	byhj::DirLight dirLight;
	byhj::PointLight pLight[PointLightSize];
	byhj::Material mat;
};

CALL_MAIN(DiffuseApp);

void  DiffuseApp::v_Init()
{

	glEnable(GL_CULL_FACE);
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
	glm::mat4 model = glm::translate(mone, glm::vec3(-5.0f, 0.0f, -10.0f) );
		//* glm::rotate(mone, glm::radians(time), glm::vec3(0.0f, 1.0f, 0.0f) );
	static float currentTime = 0.0f;
	currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	deltaTime = currentTime - lastFrame;
	deltaTime *= 10.0f;
	lastFrame = currentTime;

	//glm::mat4 view = glm::lookAt(g_Camera.GetCamPos(), g_Camera.GetCamTarget(), g_Camera.GetCamUp());
	//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), g_Aspect, 1.0f, 100.0f);

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
		glm::vec3( 3.0f, -1.0f, FieldDepth * cosf(currentTime) / 2.0f),
		glm::vec3( -3.0f, -1.0f, FieldDepth * sinf(currentTime) / 2.0f),
	};
	glm::vec3 pointLightColors[2] = 
	{
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
	};

	// Point light 1
	glUniform3f(glGetUniformLocation(program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);		
	glUniform3f(glGetUniformLocation(program, "pointLights[0].Light.ambient"), pointLightColors[0].x ,  pointLightColors[0].y ,  pointLightColors[0].z );		
	glUniform3f(glGetUniformLocation(program, "pointLights[0].Light.diffuse"), pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z); 
	glUniform3f(glGetUniformLocation(program, "pointLights[0].Light.specular"), pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
	glUniform1f(glGetUniformLocation(program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "pointLights[0].quadratic"), 0.032);		

	// Point light 2				 
	glUniform3f(glGetUniformLocation(program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);		
	glUniform3f(glGetUniformLocation(program, "pointLights[1].Light.ambient"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);		
	glUniform3f(glGetUniformLocation(program, "pointLights[1].Light.diffuse"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z); 
	glUniform3f(glGetUniformLocation(program, "pointLights[1].Light.specular"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
	glUniform1f(glGetUniformLocation(program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "pointLights[1].quadratic"), 0.032);	

	// SpotLight
	glUniform3f(glGetUniformLocation(program, "spotLight.pLight.position"), 3.0f, 1.0f, 3.0f);
	glUniform3f(glGetUniformLocation(program, "spotLight.direction"), 1.0f, -1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(program, "spotLight.pLight.Light.ambient"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(program, "spotLight.pLight.Light.diffuse"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(program, "spotLight.pLight.Light.specular"), 1.0f, 0.0f, 0.0f);
	glUniform1f(glGetUniformLocation(program, "spotLight.pLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "spotLight.pLight.linear"), 0.9);
	glUniform1f(glGetUniformLocation(program, "spotLight.pLight.quadratic"), 0.32);
	glUniform1f(glGetUniformLocation(program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
	CheckDebugLog();

	glDrawArrays(GL_TRIANGLES, 0, 6);

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



void DiffuseApp::init_buffer()
{

	//vbo are buffers that can be stored in video memory and provide the shortest access time to the GPU
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	dirLight.ambient  = glm::vec3(1.0f);
	dirLight.diffuse  = glm::vec3(1.0f);
	dirLight.specular = glm::vec3(1.0f);
	mat.ambient    = glm::vec3(0.1f);
	mat.diffuse    = glm::vec3(0.5f);
	mat.specular   = glm::vec3(1.0f);
	mat.shininess  = 32.0f;  
}

void DiffuseApp::init_vertexArray()
{
	//vao is manage the opengl status
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//We bind the buffer, change vao status
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

void DiffuseApp::init_shader()
{
	AppShader.init();
	AppShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	AppShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	AppShader.link();
	AppShader.use();

	program = AppShader.GetProgram();

	proj_loc            = glGetUniformLocation(program, "proj");  
	view_loc            = glGetUniformLocation(program, "view");  
	model_loc           = glGetUniformLocation(program, "model");
	tex_loc             = glGetUniformLocation(program, "tex");
	viewPos_loc         = glGetUniformLocation(program, "viewPos");

	dirLight.ambient_loc  = glGetUniformLocation(program, "dirLight.ambient");
	dirLight.diffuse_loc  = glGetUniformLocation(program, "dirLight.diffuse");
	dirLight.specular_loc = glGetUniformLocation(program, "dirLight.specular");

	mat.ambient_loc    = glGetUniformLocation(program, "mat.ambient");
	mat.diffuse_loc    = glGetUniformLocation(program, "mat.diffuse");
	mat.specular_loc   = glGetUniformLocation(program, "mat.specular");
	mat.shininess_loc  = glGetUniformLocation(program, "mat.shininess");

	AppShader.interfaceInfo();
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
		dirLight.ambient += glm::vec3(0.1f);
		break;
	case 'k':
		dirLight.ambient -= glm::vec3(0.1f);
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

