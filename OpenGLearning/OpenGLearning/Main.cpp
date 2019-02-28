#include "libs.h"
void FrameBufferResize(GLFWwindow* window, int fbw, int fbh)
{
	glViewport(0, 0, fbw, fbh);
}
int main()
{
	//INIT GLFW
	glfwInit();

	//CREATE WINDOW
	const int WINDOW_WIDTH = 640;
	const int WINDOW_WIEGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);

	
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_WIEGHT,"Title Used",NULL,NULL);
	glfwSetFramebufferSizeCallback(window, FrameBufferResize);
	/*glfwGetFramebufferSize(window, &framebufferWidth,&framebufferHeight);
	glViewport(0, 0,framebufferWidth,framebufferWidth );*/
	glfwMakeContextCurrent(window);//IMPORTANT

	//INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)	
	glewExperimental = GL_TRUE;
	//ERROR
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR:MAIN>CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
	// MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//Update Input---
		glfwPollEvents();
		//UPdate--

		//Draw---
		//Clear
		glClearColor(0.5f, 1.f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//DRAW

		//End Draw
		glfwSwapBuffers(window);
		glFlush();
	}
	// END OF PROGRAM
	glfwTerminate();
	return 0;
}