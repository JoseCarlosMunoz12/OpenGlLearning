#include "libs.h"
float Rotate = 0.f;
//void 
Vertex vertices[] =
{
	//Position                    //Color                     //TexCoords			 //Normals
	glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),  glm::vec3(0.0f,0.0f, 1.0f),
	glm::vec3(-0.5f,-0.5f, 0.f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.0f),  glm::vec3(0.0f,0.0f, 1.0f),
	glm::vec3( 0.5f,-0.5f, 0.f), glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.0f),  glm::vec3(0.0f,0.0f, 1.0f),
	glm::vec3( 0.5f, 0.5f, 0.f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(1.0f,1.0f),  glm::vec3(0.0f,0.0f, 1.0f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
	0,1,2, //Trianlge 1
	0,2,3  //Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void updateInput(GLFWwindow* window, Mesh &mesh)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, -0.01f));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, 0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(-0.01f, 0.f, 0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.01f, 0.f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mesh.rotate(glm::vec3(0.f,-1.f, 0.f));
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		mesh.rotate(glm::vec3(0.f, 1.f, 0.f));

	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.01f, 0.0f));

	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f,-0.01f, 0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		mesh.scaleUp(glm::vec3(0.1f));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		mesh.scaleUp(glm::vec3(-0.1f));
	}
}


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

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_WIEGHT, "Title Used", NULL, NULL);
	glfwGetFramebufferSize(window, &framebufferWidth,&framebufferHeight);
	glfwSetFramebufferSizeCallback(window, FrameBufferResize);
	
	//glViewport(0, 0,framebufferWidth,framebufferWidth );
	glfwMakeContextCurrent(window);//IMPORTANT

	//INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)	
	glewExperimental = GL_TRUE;
	//ERROR
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR:MAIN>CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//Shader INIT-----
	Shader core_program("vertex_core.glsl","fragment_core.glsl");
	/*GLuint core_program;
	if (!loadShaders(core_program))
	{
		glfwTerminate();
	}*/
	//MODELMESH----------
	Mesh test(vertices, nrOfVertices, indices, nrOfIndices);
	//Texture INIT----------
	Texture texture0("Images/Test1.png", GL_TEXTURE_2D, 0);
	//Second Texture
	Texture texture1("Images/pusheen.png",GL_TEXTURE_2D,1);
	//Material
	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		texture0.getID(), texture1.getID());

	//INT Matrices ------------
	
	glm::mat4 ViewMatrix(1.f);
	glm::vec3 camPosition(0.f,0.f,1.f);
	glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f);

	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 1000.f;
	glm::mat4 Projectionmatrix(1.f);

	Projectionmatrix = glm::perspective(glm::radians(fov),
		static_cast<float>(framebufferWidth) / framebufferHeight,
		nearPlane, farPlane);
	//Lights
	glm::vec3 LightPos0(0.f, 0.f, 1.f);

	//INIT UNIFORMS
	core_program.use();
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(Projectionmatrix, "ProjectionMatrix");

	core_program.setVec3f(LightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPos");

	// MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//Update Input---
		glfwPollEvents();
		//UPDATE--
		updateInput(window);
		updateInput(window,test);
	
		//DRAW---
		//Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//Update uniforms
		core_program.set1i(0, "texture0");
		core_program.set1i(1, "texture1");
		material0.sendToShader(core_program);
		//Update FramgeBuffer size and projection matrix
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		Projectionmatrix = glm::mat4(1.f);
		Projectionmatrix = glm::perspective(glm::radians(fov),
			static_cast<float>(framebufferWidth) / framebufferHeight,
			nearPlane, farPlane);
		core_program.setMat4fv(Projectionmatrix, "ProjectionMatrix");

		//Use a Program
		core_program.use();

		//Activate Texture
		texture0.bind();
		texture1.bind();

		//Draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		test.render(&core_program);
		//End Draw
		glfwSwapBuffers(window);
		glFlush();
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// END OF PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}