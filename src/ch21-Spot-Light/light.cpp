#include "light.h"

#include "ogl/Utility.h"

namespace byhj
{

void Light::Init()
{
	m_DirLight.ambient = glm::vec3(0.1f);
	m_DirLight.diffuse = glm::vec3(0.1f);
	m_DirLight.specular = glm::vec3(0.5f);
	m_Mat.ambient   = glm::vec3(0.1f);
	m_Mat.diffuse   = glm::vec3(0.5f);
	m_Mat.specular  = glm::vec3(1.0f);
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

	/*
	assert(m_DirLight.ambient_loc != INVALID_UNIFORM_LOCATION);
	assert(m_DirLight.diffuse_loc != INVALID_UNIFORM_LOCATION);
	assert(m_Mat.ambient_loc != INVALID_UNIFORM_LOCATION);
	assert(m_Mat.diffuse_loc != INVALID_UNIFORM_LOCATION);
	*/
}

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;

void Light::SetUniform(const GLuint &program, GLfloat deltaTime)
{
	glUniform3fv(m_DirLight.ambient_loc, 1, &m_DirLight.ambient[0]);
	glUniform3fv(m_DirLight.diffuse_loc, 1, &m_DirLight.diffuse[0]);
	glUniform3fv(m_Mat.ambient_loc,   1, &m_Mat.ambient[0]);
	glUniform3fv(m_Mat.diffuse_loc,   1, &m_Mat.diffuse[0]);
	// Positions of the point lights
	glm::vec3 pointLightPositions[2] = 
	{
		glm::vec3(5.0f, 1.0f,  FieldDepth * cosf(deltaTime)),
		glm::vec3(3.0f, 1.0f,  FieldDepth * sinf(deltaTime)),
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
	glUniform1f(glGetUniformLocation(program, "pointLights[0].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(program, "pointLights[0].quadratic"), 0.032f);		

	// Point light 2				
	glUniform3f(glGetUniformLocation(program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);		
	glUniform3f(glGetUniformLocation(program, "pointLights[1].ambient"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);		
	glUniform3f(glGetUniformLocation(program, "pointLights[1].diffuse"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z); 
	glUniform3f(glGetUniformLocation(program, "pointLights[1].specular"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
	glUniform1f(glGetUniformLocation(program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[1].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(program, "pointLights[1].quadratic"), 0.032f);	

	// SpotLight
	glUniform3f(glGetUniformLocation(program, "spotLight.position"), 3.0f, 1.0f, 10.0f);
	glUniform3f(glGetUniformLocation(program, "spotLight.direction"), 1.0f, -1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(program, "spotLight.ambient"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(program, "spotLight.diffuse"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(program, "spotLight.specular"), 1.0f, 0.0f, 0.0f);
	glUniform1f(glGetUniformLocation(program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "spotLight.linear"), 0.9f);
	glUniform1f(glGetUniformLocation(program, "spotLight.quadratic"), 0.32f);
	glUniform1f(glGetUniformLocation(program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
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