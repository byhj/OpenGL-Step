#ifndef COMMON_H
#define COMMON_H

#include <gl/glew.h>

void videoCardInfo()
{
	const GLubyte *renderer = glGetString( GL_RENDERER );  
	const GLubyte *vendor = glGetString( GL_VENDOR );  
	const GLubyte *version = glGetString( GL_VERSION );  
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );  
	GLint major, minor;  
	glGetIntegerv(GL_MAJOR_VERSION, &major);  
	glGetIntegerv(GL_MINOR_VERSION, &minor);  
	std::cout << "GL Vendor    : " << vendor << std::endl;  
	std::cout << "GL Renderer  : " << renderer << std::endl;  
	std::cout << "GL Version   : " << version << std::endl;  
	std::cout << "GL Version   : " << major << "." << minor << std::endl;  
	std::cout << "GLSL Version : " << glslVersion << std::endl;    
	std::cout << "------------------------------------------------------------------------------" << std::endl;
}

namespace Color
{
	GLfloat black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
}

#endif // !COMMON_H
