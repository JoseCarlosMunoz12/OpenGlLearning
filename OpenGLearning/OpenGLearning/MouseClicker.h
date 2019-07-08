#pragma once
#include "libs.h"

class Mouse
{
private:
	double lastMouseX;
	double lastMouseY;
	double MouseX;
	double MouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;
	float Amount = 0;
	float AmountZ = 0;
public:
	Mouse()
	{

	}
	~Mouse()
	{

	}


};