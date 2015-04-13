#ifndef BYHJ_H
#define BYHJ_H

#include <gl/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <string>
#include <functional>
#include <camera.h>
#include <SOIL/SOIL.h>

namespace byhj {
	class Application 
	{
	public:
		Application() 
		{
			camera = glm::vec3(0.0f, 0.0f, 3.0f);
			lastX = static_cast<float>(windowInfo.Width) / 2.0f;
			lastY = static_cast<float>(windowInfo.Height) / 2.0f;
			deltaTime = 0.0f;
			lastFrame = 0.0f;
			for (int i = 0; i != 1024; ++i)
			   keys[i] = false;
			firstMouse = true;
		}
		virtual ~Application() {}

		virtual void run(byhj::Application *the_app)
		{
			app = the_app;
			std::cout << "Starting GLFW context" << std::endl;
			if (!glfwInit()) 
			{
				std::cerr << "Failed to initialize GLFW" << std::endl;
				return;
			}
			initWindowInfo();
			GLFWwindow *window = glfwCreateWindow(windowInfo.Width, windowInfo.Height, windowInfo.title.c_str(), nullptr, nullptr);
			glfwSetWindowPos(window, windowInfo.posX, windowInfo.posY);
			glfwMakeContextCurrent(window);

			glfwSetKeyCallback(window, glfw_key);
			glfwSetCursorPosCallback(window, glfw_mouse);
			glfwSetScrollCallback(window, glfw_scroll);

			if (window == NULL)
			{
				std::cerr << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return ;
			}	
			glewExperimental = GL_TRUE;

			//1、查看GLSL和OpenGL的版本  
			if (glewInit() != GLEW_OK)
			{
				std::cerr << "Failed to initialize GLEW" << std::endl;
				return ;
			}
			const GLubyte *renderer = glGetString( GL_RENDERER );  
			const GLubyte *vendor = glGetString( GL_VENDOR );  
			const GLubyte *version = glGetString( GL_VERSION );  
			const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );  
			GLint major, minor;  
			glGetIntegerv(GL_MAJOR_VERSION, &major);  
			glGetIntegerv(GL_MINOR_VERSION, &minor);  
			std::cout << "GL Vendor    :" << vendor << std::endl;  
			std::cout << "GL Renderer  : " << renderer << std::endl;  
			std::cout << "GL Version (std::string)  : " << version << std::endl;  
			std::cout << "GL Version (integer) : " << major << "." << minor << std::endl;  
			std::cout << "GLSL Version : " << glslVersion << std::endl;    
			std::cout << "------------------------------------------------------------------------------" << std::endl;
			
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major); //opengl 4.3
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //using opengl core file
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
			// Create a GLFWwindow object that we can use for GLFW's functions

			init();
			glViewport(0, 0, windowInfo.Width, windowInfo.Height);

			while (!glfwWindowShouldClose(window)) 
			{
				GLfloat currentFrame = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
				glfwPollEvents();
				do_movement();

				render();

				glfwSwapBuffers(window);
			}
			shotdown();
			glfwTerminate();
		}//run
		void do_movement();
		virtual void initWindowInfo(){}
		virtual void init(){}
		virtual void render(){}
		virtual void shotdown(){}
		virtual void key_callback(GLFWwindow * window,  int key, int scancode, int action, int mode);
		virtual void mouse_callback(GLFWwindow *window, double xpos, double ypos);
		virtual void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
	protected:
		struct WindowInfo
		{
			WindowInfo():title("OpenGL-Step-by-Step"), Width(1300), Height(900), posX(300), posY(50)
			{
				aspect = static_cast<float>(Width) / static_cast<float>(Height);
			}
			std::string title;
			int Width;
			int Height;
			int posX, posY;
			float aspect;
		}windowInfo;

	protected:
		GLuint loadTexture(GLchar* path, GLboolean alpha)
		{
			//Generate texture ID and load texture data 
			GLuint textureID;
			glGenTextures(1, &textureID);
			int width,height;
			unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
			if (!image)
				std::cout << "Fail to load the textuer" << std::endl;
			// Assign texture to ID
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);	

			// Parameters
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );	// Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat 
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			SOIL_free_image_data(image);
			return textureID;
		}

		//declare the static member outside the class
		static byhj::Application *app;
		static void glfw_key(GLFWwindow * window, int key, int scancode, int action, int mode) 
		{
			app->key_callback(window,  key,  scancode, action,  mode);
		}
		static void glfw_mouse(GLFWwindow *window, double xpos, double ypos) 
		{
			app->mouse_callback(window, xpos, ypos);
		}
		static void glfw_scroll(GLFWwindow *window, double xoffset, double yoffset) 
		{
			app->scroll_callback(window, xoffset, yoffset);
		}
	protected:
		bool keys[1024];
		Camera camera;
		GLfloat lastX;
		GLfloat lastY;
		GLfloat deltaTime;
		GLfloat lastFrame;
		bool firstMouse;
	};  //class
}  //namespace 
void byhj::Application::do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void byhj::Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << std::endl;
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if(action == GLFW_PRESS)
		keys[key] = true;
	else if(action == GLFW_RELEASE)
		keys[key] = false;	
}

void byhj::Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset / 5.0, yoffset / 5.0);
}

void byhj::Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset / 10.0);
}

byhj::Application *byhj::Application::app; //静态成员需要类外声明

#define DECLARE_MAIN(a)                             \
	int main(int argc, const char **argv)           \
{                                                   \
	a *app = new a;                                 \
	app->run(app);                                  \
	delete app;                                     \
	return 0;                                       \
}


#endif  //SB6_H