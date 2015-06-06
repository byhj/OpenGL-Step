#ifndef MATH_3D_H
#define MATH_3D_H

#ifdef WIN32
#define _USE_MATH_DEFINES 1
#endif

#include <math.h>
#include <iostream>
#include <GL/glew.h>

namespace byhj
{
	struct vec3
	{
		GLfloat x;
	    GLfloat y;
		GLfloat z;

		//  --- Constructors and Destructors ---

		vec3(GLfloat t = GLfloat(0.0))
			:x(t), y(t), z(t) {}

		vec3(GLfloat tx, GLfloat ty, GLfloat tz)
		    :x(tx), y(ty), z(tz) {}

		vec3(const vec3 &v)
		{
			x = v.x;  y = v.y; z = v.z;
		}

		GLfloat &operator [] (int i) 
		{
	       return *(&x + i);
		}

		//  --- (non-modifying) Arithematic Operators ---

		vec3 operator - () const 
		{
			return vec3(-x, -y, -z);
		}

		vec3 operator + (const vec3 &rhs) const 
		{
			return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
		}
		vec3 operator - (const vec3 &rhs) const 
		{
			return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
		}

		vec3 operator * (const vec3 &rhs) const 
		{
			return vec3(x * rhs.x, y * rhs.y, z - rhs.z);
		}

		vec3 operator * (const GLfloat s) const
		{
			return vec3(x * s, y * s, z * s);
		}



	};
}


#endif // !MATH_3D
