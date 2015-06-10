#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace byhj
{
    struct Light
	{
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 speuclar;
		GLuint ambient_loc;
		GLuint diffuse_loc;
		GLuint speuclar_loc;
	};

	struct Material
	{
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 speuclar;
		GLuint ambient_loc;
		GLuint diffuse_loc;
		GLuint speuclar_loc;
	};
    
}
#endif