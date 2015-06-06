#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <GL/freeglut.h>

class Camera
{
public:
	Camera()
	{
		m_pos = glm::vec3(0.0f, 0.0f, 1.0f);
		m_target = glm::vec3(0.0f, 0.0f, 0.0f);
		m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_direction = m_target - m_pos;
	}
	Camera(const glm::vec3 &Pos, const glm::vec3 &Target, const glm::vec3 &Up)
		:m_pos(Pos), m_target(Target), m_up(Up) 
	{
		m_direction = m_target - m_pos;
	
	}

	~Camera() { };

	Camera& operator = (const Camera &camTemp)
	{
		m_pos = camTemp.m_pos;
		m_target = camTemp.m_target;
		m_up = camTemp.m_up;
		return (*this);
	}

	glm::vec3 GetCamPos() const 
	{
		return m_pos;
	}
	glm::vec3 GetCamTarget() const 
	{
		return m_target;
	}
	glm::vec3 GetCamUp() const 
	{
		return m_up;
	}
	
	void OnKeyboard(int key, GLfloat step)
	{
		switch (key)
		{
		    case GLUT_KEY_UP:
			{
				m_pos += m_direction * step;
			}
			break;

			case GLUT_KEY_DOWN:
			{
				m_pos -= m_direction  * step;
			}
			break;

			case GLUT_KEY_LEFT:
			{
				glm::vec3 right  = glm::cross(m_direction, m_up);
				right = glm::normalize(right);
				m_pos += right * step;
			}
			break;

			case GLUT_KEY_RIGHT:
			{
				m_pos -= glm::normalize(glm::cross(m_direction, m_up)) * step;
			}
			break;
		}
	}
private:
	glm::vec3 m_pos;
	glm::vec3 m_target;
	glm::vec3 m_up;
	glm::vec3 m_direction;
};

#endif