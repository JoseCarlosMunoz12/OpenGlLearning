#pragma once

#include "libs.h"

void FrameBufferResize(GLFWwindow* window, int fbw, int fbh)
{
	glViewport(0, 0, fbw, fbh);
};
class Game
{
private:
	//Variables
	//window
	GLFWwindow* window;
	const int Window_Width;
	const int Window_Height;
	int frameBufferWidth;
	int frameBufferHeight;
	//OpenGl context
	const int GLVerMajor;
	const int GLVerMinor;

	//Private Functions
	void initGLFW();
	void initWindow(const char* title,bool resizable);
	void initGLEW();
	//Static variables
public:
	//Constructors/destructors
	Game(const char * title,
		const int width, const int height,
		int GLmajorVer, int GLminorVer, bool resizable);

	virtual ~Game();
	//Accessors
	//Modifiers
	//Functions
	void update();
	void render();
	//Static Functions
};

