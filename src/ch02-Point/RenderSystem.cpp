#include "RenderSystem.h"


namespace byhj
{

void RenderSystem::v_InitInfo()
{
	windowInfo.title += "ch02-Point";
}

void RenderSystem::v_Init()
{
	m_Point.Init();
}

void RenderSystem::v_Render()
{
	static const GLfloat bgColor[4] = { 0.2f, 0.3f, 0.4f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, bgColor);

	m_Point.Render();
}

void RenderSystem::v_Shutdown()
{
	m_Point.Shutdown();
}

}