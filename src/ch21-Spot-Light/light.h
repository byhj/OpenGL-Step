#ifndef LIGHT_H
#define LIGHT_H

#include "ogl/oglLight.h"

namespace byhj
{


class Light
{
public:
   Light() {}
   ~Light() {}

public:
	void Init();
    void SetUniform(const GLuint &program, GLfloat deltaTime);
	void GetUniform(const GLuint &program);
	void AddAmbient();
	void MinusAmbient();

private:

	byhj::DirLight m_DirLight;
	byhj::Material m_Mat;
};


}
#endif