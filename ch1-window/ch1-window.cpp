#include <gl/freeglut.h>

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); //设置背景色为黑色
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);  //使用设置好的背景色
	glFlush(); //强制绘图，去掉会变白色
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA); //显示模式，重要
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(720, 640);
	glutCreateWindow("Tutorial 1 - Create a window");
	init();
	glutDisplayFunc(display);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}