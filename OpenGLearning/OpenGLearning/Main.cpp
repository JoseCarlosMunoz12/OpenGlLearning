#include "libs.h"
float Rotate = 0.f;
//void 
Vertex vertices[] =
{
	//Position                    //Color                     //TexCoords			 //Normals
	glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),  glm::vec3(0.0f,0.0f,-1.0f),
	glm::vec3(-0.5f,-0.5f, 0.0f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.0f),  glm::vec3(0.0f,0.0f,-1.0f),
	glm::vec3( 0.5f,-0.5f, 0.0f), glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.0f),  glm::vec3(0.0f,0.0f,-1.0f),
	glm::vec3( 0.5f, 0.5f, 0.0f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(1.0f,1.0f),  glm::vec3(0.0f,0.0f,-1.0f)
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

void updateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position.z -= 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position.z += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position.x -= 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position.x += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotation.y -= 2.f;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotation.y += 2.f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
 		position.y += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
	{
		position.y -= 0.01f;
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
	//MODEL----------

	//VAP,VBO, EBO-----

	//GEN VAO and BIND
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//GEN VBO AND BIND AND SEND DATA
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//GEN EBO and BIND And Send Data---------
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Set VerttexAttribPointers and Enable (input assembly)
	//Position
   	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	//TexCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);
	//BIND VAO 0
	glBindVertexArray(0);


	//Texture INIT----------
	Texture texture0("Images/Test1.png", GL_TEXTURE_2D, 0);
	//Second Texture
	Texture texture1("Images/Untitled1.png",GL_TEXTURE_2D,1);
	//Material
	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(10.f),
		texture0.getID(), texture1.getID());

	//INT Matrices ------------
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);

	glm::mat4 ModelMatrix(1.f);
	ModelMatrix = glm::translate(ModelMatrix,position);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f,0.f,0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix, scale);
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
	core_program.setMat4fv(ModelMatrix, "ModelMatrix");
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
		updateInput(window, position, rotation, scale);
	
		//DRAW---
		//Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//Update uniforms
		core_program.set1i(0, "texture0");
		core_program.set1i(1, "texture1");
		material0.sendToShader(core_program);
	
		//Move, rotate and scale
		glm::mat4 ModelMatrix(1.f);
		ModelMatrix = glm::translate(ModelMatrix, position);
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, scale);
	

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		Projectionmatrix = glm::mat4(1.f);
		Projectionmatrix = glm::perspective(glm::radians(fov),
			static_cast<float>(framebufferWidth) / framebufferHeight,
			nearPlane, farPlane);
		core_program.setMat4fv(ModelMatrix, "ModelMatrix");
		core_program.setMat4fv(Projectionmatrix, "ProjectionMatrix");

		//Use a Program
		core_program.use();

		//Activate Texture
		texture0.bind();
		texture1.bind();

		//Bind Vertex Array object
		glBindVertexArray(VAO);

		//Draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

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