#include "RenderSystem.h"
#include "ogl/Utility.h"
#include <glm/glm.hpp>

namespace byhj
{

void RenderSystem::v_InitInfo()
{
	windowInfo.title += "ch10-Index-Draw";
}

void RenderSystem::v_Init()
{
	glEnable(GL_CULL_FACE);
	m_Triangle.Init();
}

void RenderSystem::v_Render()
{
	static const GLfloat bgColor[4] = { 0.2f, 0.3f, 0.4f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, bgColor);

	ogl::MvpMatrix matrix;
	matrix.proj = glm::perspective(45.0f, GetAspect(), 0.1f, 1000.0f);
	m_Triangle.Render(matrix);

}

void RenderSystem::v_Shutdown()
{
	m_Triangle.Shutdown();
}

}