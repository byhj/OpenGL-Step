#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ogl/oglApp.h"
#include "ogl/camera.h"
#include "Cube.h"

namespace byhj
{

class RenderSystem: public byhj::Application
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

	byhj::Camera  m_Camera;
	byhj::Cube m_Cube;

};

}//Namespace

#endif