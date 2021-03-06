#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace byhj
{
    struct Light
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		GLuint ambient_loc;
		GLuint diffuse_loc;
		GLuint specular_loc;
	};

	struct DirLight
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		GLuint ambient_loc;
		GLuint diffuse_loc;
		GLuint specular_loc;
	};
	struct PointLight
	{
        DirLight dLight;

		glm::vec3 position;
		float constant;
		float linear;
		float quadratic;

		GLuint position_loc;
		GLuint constant_loc;
		GLuint linear_loc;
		GLuint quadratic_loc;
	};
	struct Material
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		GLfloat shininess;
		GLuint ambient_loc;
		GLuint diffuse_loc;
		GLuint specular_loc;
		GLuint shininess_loc;
	};
    
}
#endif