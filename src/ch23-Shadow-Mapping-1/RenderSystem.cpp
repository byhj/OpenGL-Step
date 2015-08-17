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
	glDepthFunc(GL_LESS);

	m_Camera.SetPos( glm::vec3(0.0f, 1.0f, 150.0f) );

	m_Object.Init();
	m_Plane.Init();
	init_fbo();

}

void RenderSystem::v_Render()
{
	float time = static_cast<float>( glfwGetTime() );
	static const GLfloat bgColor[4] ={ 0.2f, 0.3f, 0.4f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, bgColor);
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_DEPTH, 0, &one);

	update();

	glm::mat4 proj  = glm::perspective(m_Camera.GetZoom(), GetAspect(), 0.1f, 1000.0f);
	glm::mat4 view  = m_Camera.GetViewMatrix();
	glm::mat4 model =  glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0f, 1.0f, 0.0f) )
		            *  glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -5.0f, -10.0f));

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_Object.Render(model, view, proj);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_Plane.Render(shadow_tex);

}

void RenderSystem::v_Shutdown()
{
	m_Object.Shutdown();
}


void RenderSystem::init_fbo()
{
	
	glGenTextures(1, &shadow_tex);
	glBindTexture(GL_TEXTURE_2D, shadow_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GetScreenWidth(), GetScreenHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_tex, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// Disable writes to the color buffer
    glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) 
	{
		printf("FB error, status: 0x%x\n", Status);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
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

	m_Camera.update(deltaTime * 20.0f);
}

}