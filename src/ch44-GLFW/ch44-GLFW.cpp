#include <common/stepApp.h>

class WindowApp: public byhj::Application
{
public:
	WindowApp() 
	{
		windowInfo.title += "ch01-Window";
	}
	~WindowApp() {}

	void v_Init()
	{
		//set the background color 
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	}
	void v_Render()
	{
		//clear the color buffer to backgroud color
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

CALL_MAIN(WindowApp);
