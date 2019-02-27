#include "libs.h"
int main()
{
	int d;
	glewExperimental = GL_TRUE;
	if (glewInit != GLEW_OK)
		std::cout << "BASD" << "\n";
	else
		std::cout << "MAN";
	std::cin >> d;

	return 0;
}