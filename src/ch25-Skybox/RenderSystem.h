#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ogl/App.h"
#include "ogl/camera.h"
#include "skybox.h"

namespace byhj
{

class RenderSystem: public ogl::App
{
public:
	RenderSystem() {}
	~RenderSystem() {}

public:
	void v_InitInfo();
	void v_Init();
	void v_Render();
	void v_Shutdown();

	/////////////////////////////////Key and Mouse//////////////////////////////////
	void v_Movement(GLFWwindow *window);
	void v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void v_MouseCallback(GLFWwindow* window, double xpos, double ypos);
	void v_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	void update();

	ogl::Camera  m_Camera;
	byhj::Skybox m_Skybox;

};

}//Namespace

#endif