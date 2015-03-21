#include "mesh.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
    clear();
}

void Mesh::clear()
{
    for (unsigned int i = 0 ; i < Textures.size() ; i++) 
        SAFE_DELETE(Textures[i]);
}

bool Mesh::loadMesh(const char *filename) //��ģ���ļ��м���ģ����Ϣ
{
	clear();
	bool ret = false;
	Assimp::Importer importer; //���س������
	const aiScene *scene = importer.ReadFile(filename,
    aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if (scene)
		ret = initFromScene(scene, filename); //��ģ���ļ���ʼ������
	else
		printf("Error parsing '%s': '%s'\n", filename, importer.GetErrorString());\
		//����ʧ��
	return ret;
}

bool Mesh::initFromScene(const aiScene *scene, const char *filename) //��ʼ��ģ���ļ���Ϣ
{
	Entries.resize(scene->mNumMeshes);  //�ļ��ж��ٸ�ʵ��ģ��
	Textures.resize(scene->mNumMaterials); //�ļ���ʹ���˶�������

	for (unsigned int i = 0; i != Entries.size(); ++i) { //��ʼ��ÿһ��ʵ��ģ��
		const aiMesh * pMesh = scene->mMeshes[i];
		initMesh(i, pMesh);
	}
	bool ret = initMaterial(scene, filename); //��ʼ������
	return ret;
}

void Mesh::initMesh(unsigned int index, const aiMesh* pMesh)
{
	Entries[index].materialIndex = pMesh->mMaterialIndex;
	std::vector<Vertex> VertexData;
	std::vector<unsigned int> ElementData;
	const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < pMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(pMesh->mVertices[i]); //���㣬���ߣ���������
        const aiVector3D* pNormal   = &(pMesh->mNormals[i]);
        const aiVector3D* pTexCoord = pMesh->HasTextureCoords(0) ? &(pMesh->mTextureCoords[0][i]) : &zero3D;

        Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
			glm::vec2(pTexCoord->x, pTexCoord->y),
			glm::vec3(pNormal->x, pNormal->y, pNormal->z));

        VertexData.push_back(v);
	}
		//������������
   for (unsigned int i = 0 ; i < pMesh->mNumFaces ; i++) {
        const aiFace& Face = pMesh->mFaces[i];
        assert(Face.mNumIndices == 3); //ÿ��������Ƭ��
		ElementData.push_back(Face.mIndices[0]);
		ElementData.push_back(Face.mIndices[1]);
        ElementData.push_back(Face.mIndices[2]);
    }

    Entries[index].init(VertexData, ElementData);
}

bool Mesh::initMaterial(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) 
        Dir = ".";
    else if (SlashIndex == 0) 
        Dir = "/";
    else 
        Dir = Filename.substr(0, SlashIndex);

    bool Ret = true;
    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];
        Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath = Dir + "/" + Path.data;
                Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!Textures[i]->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete Textures[i];
                    Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("Loaded texture '%s'\n", FullPath.c_str());
                }
            }
        }

        // Load a white texture in case the model does not include its own texture
        if (!Textures[i]) {
            Textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");

            Ret = Textures[i]->Load();
        }
    }

    return Ret;
}

void Mesh::render()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0 ; i < Entries.size() ; i++) { //ÿһ��ʵ��ģ��
        glBindBuffer(GL_ARRAY_BUFFER, Entries[i].vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Entries[i].ibo);
        const unsigned int MaterialIndex = Entries[i].materialIndex;

        if (MaterialIndex < Textures.size() && Textures[MaterialIndex]) 
            Textures[MaterialIndex]->Bind(GL_TEXTURE0);
       //������������
        glDrawElements(GL_TRIANGLES, Entries[i].numElement, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
