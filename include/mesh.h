#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assert.h>
#include <glm/glm.hpp>
#include "texture.h"
#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff
#define INVALID_MATERIAL 0xffffffff
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

struct Vertex {
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 normal;
	Vertex() {}
	Vertex(const glm::vec3 &p, const glm::vec2 &t, const glm::vec3 &n)
		:pos(p), tex(t), normal(n) {}
};

struct MeshEntry {
	MeshEntry();
	~MeshEntry();
	void init(const std::vector<Vertex> &VertexData,
		      const std::vector<unsigned int> &ElementData);
	GLuint vbo, ibo;  //ÿһ��ģ�͵Ķ������飬����
	unsigned int numElement;
	unsigned int materialIndex;
};

MeshEntry::MeshEntry()
{
    vbo = INVALID_OGL_VALUE;
    vbo = INVALID_OGL_VALUE;
    numElement  = 0;
    materialIndex = INVALID_MATERIAL;
};

MeshEntry::~MeshEntry()  //�������������״̬
{
    if (vbo != INVALID_OGL_VALUE)
        glDeleteBuffers(1, &vbo);
    if (ibo != INVALID_OGL_VALUE)
        glDeleteBuffers(1, &ibo);
}

void MeshEntry::init(const std::vector<Vertex> &VertexData,
		             const std::vector<unsigned int> &ElementData)
{
    numElement = ElementData.size();
    glGenBuffers(1, &vbo);  //�������㻺�����
  	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * VertexData.size(), &VertexData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);  //����������������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numElement, &ElementData[0], GL_STATIC_DRAW);
}

class Mesh 
{
public :
	Mesh();
	~Mesh();
	bool loadMesh(const char * filename); //�����ļ�
	void render();   //����ģ��
private:
	bool initFromScene(const aiScene *scene, const char *filename); //��ʼ��ģ����Ϣ
	void initMesh(unsigned int index, const aiMesh* mesh); //��ʼ��Mesh
	void clear(); //���
	bool initMaterial(const aiScene* scene, const std::string &filename);
	std::vector<MeshEntry> Entries; //ģ������
	std::vector<Texture*> Textures; //��������
};

#endif