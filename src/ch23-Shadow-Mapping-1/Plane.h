#ifndef Plane_H
#define Plane_H

#include "ogl/Shader.h"
#include "ogl/model.h"

namespace byhj
{

	class Plane
	{

	public:
		void Init();
		void Render(GLuint shadowTex);
		void Shutdown();

	private:
		void init_buffer();
		void init_vertexArray();
		void init_shader();

		GLuint program = 0;
		GLuint vao, ibo, vbo;
		GLuint tex_loc;
		ogl::Shader PlaneShader ={ "Plane Shader" };
	};


}
#endif