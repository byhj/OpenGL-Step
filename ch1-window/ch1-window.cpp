#include <gl/freeglut.h>

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);  //ʹ�����úõı���ɫ
	glFlush(); //ǿ�ƻ�ͼ��ȥ������ɫ
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA); //��ʾģʽ����Ҫ
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(720, 640);
	glutCreateWindow("Tutorial 1 - Create a window");
	init();
	glutDisplayFunc(display);
	glutMainLoop(); //ѭ������ע�ắ��display
	return 0;
}