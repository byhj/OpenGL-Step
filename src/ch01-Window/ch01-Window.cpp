#include <GL/freeglut.h>

#ifdef WIN32
//If PLATFORM IS WIN32, we put the render window to the middle of window
const int g_ScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.75;
const int g_ScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.75;
const int g_PosX = (GetSystemMetrics(SM_CXSCREEN) - g_ScreenWidth) / 2;
const int g_PosY = (GetSystemMetrics(SM_CYSCREEN) - g_ScreenHeight) / 2;

#else
const int g_ScreenWidth = 800;
const int g_ScreenHeight = 600;
const int g_PosX = 300;
const int g_PosY = 100;
#endif

//Window Title
const char *g_pWindowTitle = "ch1-Window";

void init()
{
	//set the background color 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void render()
{
	//clear the color buffer to backgroud color
	glClear(GL_COLOR_BUFFER_BIT);

   //Swap the buffer to show
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_ScreenWidth, g_ScreenHeight);
	glutInitWindowPosition(g_PosX, g_PosY);
	glutCreateWindow(g_pWindowTitle);
	init();
	glutDisplayFunc(render);
	glutMainLoop();
}