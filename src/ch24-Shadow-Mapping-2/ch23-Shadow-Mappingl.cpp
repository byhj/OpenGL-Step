#include <common/stepApp.h>
#include <common/shader.h>
#include <common/camera.h>
#include <common/Texture.h>
#include <common/light.h>
#include <common/model.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;

class TriangleApp: public byhj::Application
{
public:
	TriangleApp() :TriangleShader("Triangle Shader"), camera(glm::vec3(0.0f, 50.0f, 50.0f))
	{
		windowInfo.title += "cg22-Loading-Model";
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
		init_fbo();

		glEnable(GL_DEPTH_TEST);
		//set the background color 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);

	}
	void v_Render()
	{
		//clear the color buffer to backgroud color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw the scene depth data to texture
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		//We use current shader program and vao status to render the scene
		glUseProgram(program);
		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D, tex);

		GLfloat currentTime = glfwGetTime();
		deltaTime = (currentTime - lastTime) * 10.0f;
		lastTime = currentTime;

		//You should add the translate to set last
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, -100.0f) )
		                * glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), GetAspect(), 1.0f, 1000.0f);

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
			glm::vec3(5.0f, 1.0f,  FieldDepth * cosf(currentTime) * 10),
			glm::vec3(3.0f, 1.0f, FieldDepth * sinf(currentTime) * 10),
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

		// SpotLight
		glUniform3f(glGetUniformLocation(program, "spotLight.position"), 3.0f, 1.0f, 10.0f);
		glUniform3f(glGetUniformLocation(program, "spotLight.direction"), 1.0f, -1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(program, "spotLight.ambient"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(program, "spotLight.diffuse"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(program, "spotLight.specular"), 1.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(program, "spotLight.linear"), 0.9);
		glUniform1f(glGetUniformLocation(program, "spotLight.quadratic"), 0.32);
		glUniform1f(glGetUniformLocation(program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		objModel.Draw(program);

/*
		//Draw the shadow texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		//clear the color buffer to backgroud color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, tex_shadow);
		glUseProgram(shadow_prog);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glUseProgram(0);
**/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		//clear the color buffer to backgroud color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindTexture(GL_TEXTURE_2D, tex_shadow);
		currentTime = glfwGetTime();
		deltaTime = (currentTime - lastTime) * 10.0f;
		lastTime = currentTime;

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		GLfloat near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
		lightView = glm::lookAt(glm::vec3(10.0f, 40.0f, -100.0f), glm::vec3(0.0f), glm::vec3(1.0));
		lightSpaceMatrix = lightProjection * lightView;
		glUniformMatrix4fv(glGetUniformLocation(program, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

		//You should add the translate to set last
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, -100.0f) )
			* glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		view = camera.GetViewMatrix();
		proj = glm::perspective(glm::radians(camera.Zoom), GetAspect(), 1.0f, 1000.0f);

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

		// SpotLight
		glUniform3f(glGetUniformLocation(program, "spotLight.position"), 3.0f, 1.0f, 10.0f);
		glUniform3f(glGetUniformLocation(program, "spotLight.direction"), 1.0f, -1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(program, "spotLight.ambient"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(program, "spotLight.diffuse"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(program, "spotLight.specular"), 1.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(program, "spotLight.linear"), 0.9);
		glUniform1f(glGetUniformLocation(program, "spotLight.quadratic"), 0.32);
		glUniform1f(glGetUniformLocation(program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		objModel.Draw(program);

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
	void init_fbo();

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
	Model objModel;
	Shader ShowShader;
	GLuint tex_shadow, tex_shadow_loc;
	GLuint fbo;
	GLuint shadow_prog;
};
CALL_MAIN(TriangleApp);


void TriangleApp::init_buffer()
{
	objModel.loadModel("../../media/phoenix_ugv.md2");
	dirLight.ambient  = glm::vec3(1.0f);
	dirLight.diffuse  = glm::vec3(0.5f);
	dirLight.specular = glm::vec3(1.0f);
	mat.ambient    = glm::vec3(0.5f);
	mat.diffuse    = glm::vec3(0.5f);
	mat.specular   = glm::vec3(1.0f);
	mat.shininess  = 32.0f;  
}


void TriangleApp::init_fbo()
{
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &tex_shadow);
	glBindTexture(GL_TEXTURE_2D, tex_shadow);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GetScreenWidth(), GetScreenHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_shadow, 0);
	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return;
	}
}
void TriangleApp::init_vertexArray()
{
}

void TriangleApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "model.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "model.frag");
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

	ShowShader.init();
	ShowShader.attach(GL_VERTEX_SHADER, "show.vert");
	ShowShader.attach(GL_FRAGMENT_SHADER, "show.frag");
	ShowShader.link();
	shadow_prog = ShowShader.GetProgram();

}


void TriangleApp::init_texture()
{
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
		mat.ambient += glm::vec3(0.01f);
	if(keys[GLFW_KEY_I])
		mat.ambient -= glm::vec3(0.01f);
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
