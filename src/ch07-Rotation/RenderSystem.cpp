#include "RenderSystem.h"


namespace byhj
{

void RenderSystem::v_InitInfo()
{
	windowInfo.title += "ch07-Rotation";
}

void RenderSystem::v_Init()
{
	m_Triangle.Init();
}

void RenderSystem::v_Render()
{
	static const GLfloat bgColor[4] = { 0.2f, 0.3f, 0.4f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, bgColor);

	m_Triangle.Render();
}

void RenderSystem::v_Shutdown()
{
	m_Triangle.Shutdown();
}

}