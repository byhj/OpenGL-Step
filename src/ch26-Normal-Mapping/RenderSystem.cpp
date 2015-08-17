#include "RenderSystem.h"

namespace byhj
{

void RenderSystem::v_InitInfo()
{
	windowInfo.title += "ch14-Camera";
}

void RenderSystem::v_Init()
{
	glEnable(GL_DEPTH_TEST);

	m_Camera.SetPos( glm::vec3(0.0f, 0.0f, 0.0f) );
	m_Cube.Init();
}

void RenderSystem::v_Render()
{
	static const GLfloat bgColor[4] = { 0.2f, 0.3f, 0.4f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, bgColor);
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_DEPTH, 0, &one); 

	float time = static_cast<float>( glfwGetTime() );
	update();

	glm::mat4 view = m_Camera.GetViewMatrix();
	glm::mat4 proj = glm::perspective(45.0f, GetAspect(), 0.1f, 1000.0f);
	glm::mat4 mvp =  proj * view;

	// Draw Cube first, disable depth writing. 

	glDepthFunc(GL_LEQUAL);
	m_Cube.Render(mvp);
	glDepthFunc(GL_LESS);

	// Set depth function back to default

}

void RenderSystem::v_Shutdown()
{
	m_Cube.Shutdown();
}

/////////////////////////////////Key and Mouse//////////////////////////////////
void RenderSystem::v_Movement(GLFWwindow *window)
{
	m_Camera.movement(window);
}
void RenderSystem::v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	m_Camera.key_callback(window, key, scancode, action, mode);
}

void RenderSystem::v_MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	m_Camera.mouse_callback(window, xpos, ypos);
}

void RenderSystem::v_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_Camera.scroll_callback(window, xoffset, yoffset);
}

void RenderSystem::update()
{
	static GLfloat lastFrame = static_cast<float>( glfwGetTime() );
	GLfloat currentFrame = static_cast<float>( glfwGetTime() );
	GLfloat deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	m_Camera.update(deltaTime);
}

}