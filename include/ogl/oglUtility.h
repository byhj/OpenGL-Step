#ifndef OGLUTILITY_H
#define OGLUTILITY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>

namespace byhj
{
	struct MvpMatrix
	{
	   glm::mat4 model;
	   glm::mat4 view;
	   glm::mat4 proj;
	};
}

#define ARRAY_ELEMENTS(a) ( sizeof(a)/sizeof(a[0]) )

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#endif
