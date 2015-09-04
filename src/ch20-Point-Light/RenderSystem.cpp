#include "RenderSystem.h"
#include "ogl/Utility.h"


namespace byhj
{

void RenderSystem::v_InitInfo()
{
	windowInfo.title += "ch14-Camera";
}

void RenderSystem::v_Init()
{
	glEnable(GL_CULL_FACE);

	m_Camera.SetPos( glm::vec3(0.0f, 1.0f, 8.0f) );
	m_Triangle.Init();
}

void RenderSystem::v_Render()
{
	static const GLfloat bgColor[4] = { 0.2f, 0.3f, 0.4f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, bgColor);

	float time = static_cast<float>( glfwGetTime() );
	update();

	ogl::MvpMatrix matrix;
	matrix.proj  = glm::perspective(m_Camera.GetZoom(), GetAspect(), 0.1f, 1000.0f);
	matrix.view  = m_Camera.GetViewMatrix();
	matrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, -10.0f) );

	m_Triangle.Render(matrix, m_Camera);

}

void RenderSystem::v_Shutdown()
{
	m_Triangle.Shutdown();
}

/////////////////////////////////Key and Mouse//////////////////////////////////
void RenderSystem::v_Movement(GLFWwindow *window)
{
	m_Camera.movement(window);
}
void RenderSystem::v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_Z)
		m_Triangle.AddAmbient();
	if (key == GLFW_KEY_X)
		m_Triangle.MinusAmbient();

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