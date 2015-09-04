#include "skybox.h"
#include "ogl/loadTexture.h"

namespace byhj
{

void Skybox::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Skybox::Render(const glm::mat4 &mvp)
{
	glUseProgram(program);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

	SphereModel.Draw(program);

	glUseProgram(0);
}

void Skybox::Shutdown()
{

}

void Skybox::init_buffer()
{

	SphereModel.loadModel("../../media/sphere.obj");

}

void Skybox::init_vertexArray()
{

}

void Skybox::init_shader()
{
	SkyboxShader.init();
	SkyboxShader.attach(GL_VERTEX_SHADER, "skybox.vert");
	SkyboxShader.attach(GL_FRAGMENT_SHADER, "skybox.frag");
	SkyboxShader.link();
	program = SkyboxShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "skybox");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Skybox::init_texture()
{
	// Cubemap (Skybox)
	std::vector<std::string> faces;
	faces.push_back("../../media/sp3right.jpg");
	faces.push_back("../../media/sp3left.jpg");
	faces.push_back("../../media/sp3top.jpg");
	faces.push_back("../../media/sp3bot.jpg");
	faces.push_back("../../media/sp3front.jpg");
	faces.push_back("../../media/sp3back.jpg");
	cubemap_texture = ogl::loadCubeMap(faces);
}

}