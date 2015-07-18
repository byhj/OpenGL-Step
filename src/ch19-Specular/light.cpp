#include "light.h"

#include "ogl/oglUtility.h"

namespace byhj
{

void Light::Init()
{
	m_DirLight.ambient = glm::vec3(0.5f);
	m_DirLight.diffuse = glm::vec3(0.5f);
	m_DirLight.specular = glm::vec3(1.0f);
	m_Mat.ambient   = glm::vec3(0.1f);
	m_Mat.diffuse   = glm::vec3(0.5f);
	m_Mat.specular  = glm::vec3(0.5f);
	m_Mat.shininess = 16.0f;

	m_DirLight.ambient_loc = -1;
	m_DirLight.diffuse_loc = -1;

	m_Mat.ambient_loc = -1;
	m_Mat.diffuse_loc = -1;
}

void Light::GetUniform(const GLuint &program)
{
	m_DirLight.ambient_loc = glGetUniformLocation(program, "g_Light.ambient");
	m_DirLight.diffuse_loc = glGetUniformLocation(program, "g_Light.diffuse");
	m_DirLight.specular_loc = glGetUniformLocation(program, "g_Light.specular");

	m_Mat.ambient_loc  = glGetUniformLocation(program, "g_Mat.ambient");
	m_Mat.diffuse_loc  = glGetUniformLocation(program, "g_Mat.diffuse");
	m_Mat.specular_loc = glGetUniformLocation(program, "g_Mat.specular");
	m_Mat.shininess_loc = glGetUniformLocation(program, "g_Mat.shininess");

	assert(m_DirLight.ambient_loc != INVALID_UNIFORM_LOCATION);
	assert(m_DirLight.diffuse_loc != INVALID_UNIFORM_LOCATION);
	assert(m_Mat.ambient_loc != INVALID_UNIFORM_LOCATION);
	assert(m_Mat.diffuse_loc != INVALID_UNIFORM_LOCATION);
}

void Light::SetUniform()
{
	glUniform3fv(m_DirLight.ambient_loc, 1, &m_DirLight.ambient[0]);
	glUniform3fv(m_DirLight.diffuse_loc, 1, &m_DirLight.diffuse[0]);
	glUniform3fv(m_Mat.ambient_loc,   1, &m_Mat.ambient[0]);
	glUniform3fv(m_Mat.diffuse_loc,   1, &m_Mat.diffuse[0]);
}

void Light::AddAmbient()
{
   m_DirLight.ambient += glm::vec3(0.1f);
}

void Light::MinusAmbient()
{
	m_DirLight.ambient -= glm::vec3(0.1f);
}

}