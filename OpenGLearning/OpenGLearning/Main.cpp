#include "libs.h"
int main()
{
	glm::vec3(0.0f);
	int d;
	glewExperimental = GL_TRUE;
	if (glewInit != GLEW_OK)
		std::cout << "BASD" << "\n";
	else
		std::cout << "MAN";
	std::cin >> d;

	return 0;
}