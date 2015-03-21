
#include <vector>
#include <string>
using namespace std;

char *textFileRead(const char *fn);  


class Shader {
public:
	Shader(std::string shaderName):name(shaderName) {}
	void init();
	void attach(int type, char *filename);
	void link();
	void use(void) {
		glUseProgram(program);
		
	}
	void end(void) {
		glUseProgram(0);
		
	}
	vector<GLuint> handles; //shader handle
	GLuint program;   //shader program
	string name;   //shader class name
};

char *textFileRead( char *fn) {  //read the shader code
	FILE *fp;  
	char *content = NULL;  
	int count=0;  

	if (fn != NULL) {  
		fopen_s(&fp , fn, "rt");

		if (fp != NULL) {  
			fseek(fp, 0, SEEK_END);  
			count = ftell(fp);  
			rewind(fp);  

			if (count > 0) {  
				content = (char *)malloc(sizeof(char) * (count+1));  
				count = fread(content,sizeof(char),count,fp);  
				content[count] = '\0';  
			}  
			fclose(fp);  
		} 
		else 
			cout << "Fail to open the shader file" << endl;
	}  
	return content;  
}  


void Shader::init()
{
		//1���鿴GLSL��OpenGL�İ汾  
	const GLubyte *renderer = glGetString( GL_RENDERER );  
	const GLubyte *vendor = glGetString( GL_VENDOR );  
	const GLubyte *version = glGetString( GL_VERSION );  
	const GLubyte *glslVersion =   
	glGetString( GL_SHADING_LANGUAGE_VERSION );  
	GLint major, minor;  
	glGetIntegerv(GL_MAJOR_VERSION, &major);  
	glGetIntegerv(GL_MINOR_VERSION, &minor);  
	cout << "GL Vendor    :" << vendor << endl;  
	cout << "GL Renderer  : " << renderer << endl;  
	cout << "GL Version (string)  : " << version << endl;  
	cout << "GL Version (integer) : " << major << "." << minor << endl;  
	cout << "GLSL Version : " << glslVersion << endl;   
	cout << "-----------------------------------------------------------------------" << endl;
}

void Shader::attach(int type, char* filename) //���Ӳ�ͬ����shader
{
		char* mem = textFileRead(filename);
		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, (const GLchar**)(&mem), 0);
		

		glCompileShader(handle);
		
		GLint compileSuccess=0;
		GLchar compilerSpew[256];

		glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);
		
		if (!compileSuccess) {
			glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader %s\n%s\ncompileSuccess=%d\n",filename, compilerSpew, compileSuccess);
			
			while(1);;
		}
		handles.push_back(handle); //�洢������shader handle�Թ���������
}

void Shader::link()
{
    program = glCreateProgram();
	for (int i=0; i!=handles.size(); i++) {
		glAttachShader(program, handles[i]); //��ǰ�洴����shader��ӵ�program
		
	}
	glLinkProgram(program);  //����shader program
	

	GLint linkSuccess;
	GLchar compilerSpew[256];
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess); //���������Ϣ
	if(!linkSuccess) {
			glGetProgramInfoLog(program, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader Linker:\n%s\nlinkSuccess=%d\n",compilerSpew,linkSuccess);
			
			while(1);;
	}
		printf("%s linked successful\n",name.c_str());
	
}

