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
	GLuint vbo, ibo;  //每一个模型的顶点数组，索引
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

MeshEntry::~MeshEntry()  //析构函数，清除状态
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
    glGenBuffers(1, &vbo);  //创建顶点缓存对象
  	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * VertexData.size(), &VertexData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);  //创建顶点索引对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numElement, &ElementData[0], GL_STATIC_DRAW);
}

class Mesh 
{
public :
	Mesh();
	~Mesh();
	bool loadMesh(const char * filename); //加载文件
	void render();   //绘制模型
private:
	bool initFromScene(const aiScene *scene, const char *filename); //初始化模型信息
	void initMesh(unsigned int index, const aiMesh* mesh); //初始化Mesh
	void clear(); //清除
	bool initMaterial(const aiScene* scene, const std::string &filename);
	std::vector<MeshEntry> Entries; //模型数据
	std::vector<Texture*> Textures; //纹理数据
};

#endif