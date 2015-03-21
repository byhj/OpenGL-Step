#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#include <shader.h>
#include <light.h>
#define BUFFER_OFFSET(offset) ((GLvoid*)(NULL + offset))

GLuint vbo, vao, ibo , program;
GLuint proj_matrix_loc, mv_matrix_loc;
GLuint gSampler, texture;
Shader spotLightShader("diffuse shader");

struct Vertex {
	glm::vec3 pos; //顶点，纹理，法线坐标
	glm::vec2 tex;
	glm::vec3 normal;
	Vertex(glm::vec3 mpos, glm::vec2 mtex)
	{
		pos = mpos;
		tex = mtex;
		normal = glm::vec3(0.0f, 0.0f, 0.0f);
	}
};


#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

//计算法线坐标
void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
	Vertex* pVertices, unsigned int VertexCount)
{
	// Accumulate each triangle normal into each of the triangle vertices
	for (unsigned int i = 0; i < IndexCount; i += 3) { //对每个三角形操作
		unsigned int Index0 = pIndices[i];   //获取当前三角形的三个顶点
		unsigned int Index1 = pIndices[i + 1];
		unsigned int Index2 = pIndices[i + 2]; 
		glm::vec3 v1 = pVertices[Index1].pos - pVertices[Index0].pos;
		glm::vec3 v2 = pVertices[Index2].pos - pVertices[Index0].pos;
		glm::vec3 Normal = glm::normalize(glm::cross(v1, v2));//使用差积计算每个点的法线

		pVertices[Index0].normal += Normal; //累积法线，有共享点？
		pVertices[Index1].normal += Normal;
		pVertices[Index2].normal += Normal;
	}
	// Normalize all the vertex normals
	for (unsigned int i = 0; i < VertexCount; i++) {  //对每一法线坐标进行单位化
		pVertices[i].normal = glm::normalize(pVertices[i].normal);
	}
}
Vertex VertexData[4] = { Vertex(glm::vec3(-1.0f, -1.0f, 0.5773f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(0.0f, -1.0f, -1.15475f), glm::vec2(0.5f, 0.0f)),
		Vertex(glm::vec3(1.0f, -1.0f, 0.5773f),  glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(0.0f, 1.0f, 0.0f),      glm::vec2(0.5f, 1.0f))
};

const static unsigned int ElementData[] = {
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
};

glm::vec3 gEyeWorldPos;
float gMatSpecularIntensity;
float gSpecularPower;
Lighting light;
DirectionalLight dLight;
PointLight pLight[2];
static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;
float m_scale = 0.0f;

void init_light()
{
	dLight.Color= glm::vec3(1.0f, 1.0f, 1.0f);
    dLight.AmbientIntensity = 0.0f;
    dLight.DiffuseIntensity = 0.01f;
    dLight.Direction = glm::vec3(1.0f, -1.0, 0.0);

	pLight[0].DiffuseIntensity = 0.5f;
    pLight[0].Color = glm::vec3(1.0f, 0.5f, 0.0f);
    pLight[0].Position = glm::vec3(3.0f, 1.0f, FieldDepth * (cosf(m_scale) + 1.0f) / 2.0f);
    pLight[0].Attenuation.Linear = 0.1f;
    pLight[1].DiffuseIntensity = 0.5f;
    pLight[1].Color = glm::vec3(0.0f, 0.5f, 1.0f);
    pLight[1].Position = glm::vec3(7.0f, 1.0f, FieldDepth * (sinf(m_scale) + 1.0f) / 2.0f);
    pLight[1].Attenuation.Linear = 0.1f;

	gEyeWorldPos = glm::vec3(5.0f, 1.0f, 3.0f);
	gMatSpecularIntensity = 0.0f;
	gSpecularPower = 0;
}

void load_shader()
{
	spotLightShader.init();
	spotLightShader.attach(GL_VERTEX_SHADER, "pointLight.vert");
	spotLightShader.attach(GL_FRAGMENT_SHADER, "pointLight.frag");
	spotLightShader.link();
	spotLightShader.use();
	program = spotLightShader.program;
	mv_matrix_loc = glGetUniformLocation(program, "mv_matrix");
	proj_matrix_loc = glGetUniformLocation(program, "proj_matrix");
    gSampler = glGetUniformLocation(program, "gSampler");
	glUniform1i(gSampler, 0);
	light.Init(program);
	light.SetPointLights(2, pLight);
	light.SetMatSpecularIntensity(gMatSpecularIntensity);
	light.SetMatSpecularPower(gSpecularPower);
	light.SetDirectionalLight(dLight);

}

void init_buffer()
{
	unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(VertexData);
	unsigned int IndexCount = ARRAY_SIZE_IN_ELEMENTS(ElementData);
	CalcNormals(ElementData, IndexCount, VertexData, VertexCount);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);  //将索引数据传入
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData),
		ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init_diffuse()
{
	texture = loadTexture("../test.png"); //将纹理数据传入
}

void init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL); 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void init()
{
	load_shader();
	init_diffuse();
	init_buffer();
	init_vertexArray();
	glFrontFace(GL_CW); //背面剔除
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}


static glm::vec3 target(0.0f, 0.0f, 0.0f); //控制摄像头，使用球体坐标360度旋转
static glm::vec3 up(0.0f, 1.0f, 0.0f);
static float camX = 0.0, camY = 0.0, camZ = 5.0;
int startX, startY, tracking = 0;
float alpha = 0.0f, beta = 0.0f;
float r = 5.0f;

void render()
{
	static const float black[] = {0.0f, 0.0f, 0.0f ,1.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	spotLightShader.use();
	glBindVertexArray(vao);
	glViewport(0, 0, 720, 640);
	static float scale = 0.0f;
	scale += 0.01f;  //先平移，缩放，旋转

	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -3.0f));               
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(camX, camY, camZ), target, up);
	glm::mat4 mv_matrix =  view_matrix * model_matrix ;
	glUniformMatrix4fv(mv_matrix_loc, 1, GL_FALSE, &mv_matrix[0][0]);

	glm::mat4 proj_matrix = glm::perspective(60.0f, 720.0f / 640.0f, 1.0f, 100.0f);
	glUniformMatrix4fv(proj_matrix_loc, 1, GL_FALSE, &proj_matrix[0][0]);
	gEyeWorldPos = glm::vec3(camX, camY, camZ);
	light.SetEyeWorldPos(gEyeWorldPos);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
	glutPostRedisplay();
}

void processKeys(unsigned char key, int xx, int yy) 
{
	switch(key) {

		case 27:
			glutLeaveMainLoop();
			break;
		case 'z': r -= 0.1f; break;
		case 'x': r += 0.1f; break;	
		case 'm': glEnable(GL_MULTISAMPLE); break;
		case 'n': glDisable(GL_MULTISAMPLE); break;
	}
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

//  uncomment this if not using an idle func
//	glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx; //获取当前坐标
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (startX - xx);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = startX - xx;
	deltaY = yy - startY;

	// left mouse button: move diffuse
	if (tracking == 1) {
		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;
		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;

		camX = rAux * cos(betaAux * 3.14f / 180.0f) * sin(alphaAux * 3.14f / 180.0f);
		camZ = rAux * cos(betaAux * 3.14f / 180.0f) * cos(alphaAux * 3.14f / 180.0f);
		camY = rAux * sin(betaAux * 3.14f / 180.0f);
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);

		camX = rAux * cos(betaAux * 3.14f / 180.0f) * sin(alphaAux * 3.14f / 180.0f);
		camZ = rAux * cos(betaAux * 3.14f / 180.0f) * cos(alphaAux * 3.14f / 180.0f);
		camY = rAux * sin(betaAux * 3.14f / 180.0f);
	}
}

void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);
}

int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //显示模式，重要
	glutInitWindowPosition(150, 0);
	glutInitWindowSize(720, 640);
	glutCreateWindow("Tutorial 21 - Spot Light");
	GLenum res = glewInit();
	if (res != GLEW_OK)
		cout << "Fail to initial the glew:" << glewGetString(res) << endl;
	init();
	glutDisplayFunc(render);
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc (mouseWheel) ;
	glutMainLoop(); //循环调用注册函数displa
	return 0;
}