#include "Game.h"

void Game::initGLFW()
{
	//INIT GLFW
	if (glfwInit() == GLFW_FALSE)
	{
			std::cout << "ERROR::GLFE_INIT_FAILED" << "\n";
			glfwTerminate();
	}
		
}

void Game::initWindow(const char* title, bool resizable)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GLVerMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GLVerMinor);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(Window_Width, Window_Height, title, NULL, NULL);
	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINODE_INIT_FAILED";
		glfwTerminate();
	}
	glfwGetFramebufferSize(this->window, &this->frameBufferWidth , &this->frameBufferHeight );
	glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize_callback);

	//glViewport(0, 0,framebufferWidth,framebufferWidth );
	glfwMakeContextCurrent(this->window);//IMPORTANT

}

void Game::initGLEW()
{
	//INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)	
	glewExperimental = GL_TRUE;
	//ERROR
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR:MAIN>CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glfwSetInputMode(this->window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	
	this->ProjectionMatrix = glm::mat4(1.f);

	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov),
		static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
		this->nearPlane,
		this->farPlane);

}

void Game::initShaders()
{
	this->shaders.push_back(new Shader(this->GLVerMajor , this->GLVerMinor ,
		"vertex_core.glsl", "fragment_core.glsl"));
	this->shaders.push_back(new Shader(this->GLVerMajor, this->GLVerMinor,
		"terrain_vertex.glsl", "terrain_fragment.glsl"));

}

void Game::initTextures()
{
	//Texture INIT----------
	this->textures.push_back(new Texture("Images/pusheen.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/pusheen_specular.png", GL_TEXTURE_2D));
	//Second Texture
	this->textures.push_back(new Texture("Images/container.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/container_specular.png", GL_TEXTURE_2D));
	//Third Image
	this->textures.push_back(new Texture("Images/Test1.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/Test1_specular.png", GL_TEXTURE_2D));
	
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0,1));
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0, 1));
	
}

void Game::initModels()
{
	std::vector<Mesh*> meshes;
		meshes.push_back(
		new Mesh(
			&CustomTerrain(400,200),
			glm::vec3(0.f, -1.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));
		meshes.push_back(
			new Mesh(
				&Pyramid(),
				glm::vec3(0.f, 0.f, 0.f),
				glm::vec3(0.f),
				glm::vec3(0.f),
				glm::vec3(1.f)));
		meshes.push_back(
			new Mesh(
				&Cube(),
				glm::vec3(0.f, 0.f, 5.f),
				glm::vec3(0.f),
				glm::vec3(0.f),
				glm::vec3(1.f)));
	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[1],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes[0]));
	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[0],
		this->textures[TEX_PUSHEEM],
		this->textures[TEX_PUSHEEN_SPECULAR],
		meshes[1]));
	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[0],
		this->textures[TEX_PUSHEEM],
		this->textures[TEX_PUSHEEN_SPECULAR],
		meshes[2]));
	for (auto*& i : meshes)
	{
		delete i;
	}
}

void Game::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void Game::initUniforms()
{
	for (auto& i : this->shaders)
	{
		i->setMat4fv(ViewMatrix, "ViewMatrix");
		i->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
		i->setVec3f(*this->lights[0], "lightPos0");
	}
}

void Game::updateDT()
{
	this->cuTime = static_cast<float>(glfwGetTime());
	this->dt = this->cuTime - this->lastTime;
	this->lastTime = this->cuTime;
	//std::cout << 1 / dt <<"\n";
}

void Game::updateKeyboardInput()
{
	if (glfwGetKey(this->window, GLFW_KEY_J) == GLFW_PRESS)
	{
		this->lights[0] = new glm::vec3(AmountZ, 0.f, Amount);
		AmountZ += 1;
	}
	if (glfwGetKey(this->window, GLFW_KEY_K) == GLFW_PRESS)
	{
		this->lights[0] = new glm::vec3(AmountZ, 0.f, Amount);
		AmountZ -= 1;
	}
	if (glfwGetKey(this->window, GLFW_KEY_I) == GLFW_PRESS)
	{
		this->lights[0] = new glm::vec3(AmountZ,0.f,Amount);
		Amount += 1;
	}
	if (glfwGetKey(this->window, GLFW_KEY_U) == GLFW_PRESS)
	{
		this->lights[0] = new glm::vec3(AmountZ, 0.f,Amount);
		Amount -= 1;
	}
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}
	//Camera
	if (glfwGetKey(this->window , GLFW_KEY_W) == GLFW_PRESS)
	{
		this->camera.move(this->dt, FORWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->camera.move(this->dt, BACKWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->camera.move(this->dt, LEFT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->camera.move(this->dt, RIGHT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS)
	{
		std::vector<Mesh*> meshes;
		
		meshes.push_back(
			new Mesh(
				&Cube(),
				glm::vec3((float)xDist(rng), 0.f, (float)yDist(rng)),
				glm::vec3(0.f),
				glm::vec3(0.f),
				glm::vec3(1.f)));
		this->models.push_back(new Model(
			glm::vec3(0.f),
			this->materials[0],
			this->textures[TEX_PUSHEEM],
			this->textures[TEX_PUSHEEN_SPECULAR],
			meshes[0]));
		for (auto*& i : meshes)
		{
			delete i;
		}
	}
	if (glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS)
	{
		if (this->models.size() > 3)
		{
			models.pop_back();
		}
	}
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->MouseX, &this->MouseY);
	if (this->firstMouse)
	{
		this->lastMouseX = this->MouseX;
		this->lastMouseY = this->MouseY;
		this->firstMouse = false;
	}

	//Calc offset
	this->mouseOffsetX = this->MouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->MouseY;

	//Set last X and Y
	this->lastMouseX = this->MouseX;
	this->lastMouseY = this->MouseY;
	}

void Game::updateInput()
{
	glfwPollEvents();
	this->updateOpenGLOptions();
	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt,-1,this->mouseOffsetX, this->mouseOffsetY);
}

void Game::updateUniforms()
{
	//Update uniforms
	this->ViewMatrix = this->camera.GetViewMatrix();
	for (auto& i : this->shaders)
	{
		i->setVec3f(*this->lights[0], "lightPos0");
		i->setMat4fv(this->ViewMatrix, "ViewMatrix");
		i->setVec3f(this->camera.getPosition(), "cameraPos");
		
	}
	//Update FramgeBuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	   
	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov),
		static_cast<float>(this->frameBufferWidth) / this-> frameBufferHeight,
		this->nearPlane,
		this->farPlane);
	for (auto& i : this->shaders)
	{
		i->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
	}
}

void Game::updateOpenGLOptions()
{
	if (glfwGetKey(this->window, GLFW_KEY_L) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
		
	}
	if (glfwGetKey(this->window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}
}

Game::Game(const char * title,
	const int width, const int height,
	const int GLmajorVer, const int GLminorVer, bool resizable)
	: Window_Width(width), Window_Height(height),
	GLVerMajor(GLmajorVer), GLVerMinor(GLminorVer),
	camera(glm::vec3(0.f,0.f,1.f),glm::vec3(0.f,0.f,1.f),glm::vec3(0.f,1.f,0.f)),
	rng(std::random_device()()),xDist(-100,100),yDist(-100,100)

{
	this->window = NULL;
	this->frameBufferHeight = this->Window_Height;
	this->frameBufferWidth = this->Window_Width;

	
	this-> camPosition = glm::vec3(0.f, 0.f, 1.f);
	this-> worldUp = glm::vec3(0.f, 1.f, 0.f);
	this-> camFront = glm::vec3(0.f, 0.f, -1.f);


	this-> fov = 90.f;
	this-> nearPlane = 0.1f;
	this-> farPlane = 1000.f;
	
	this->dt = 0.f;
	this->cuTime = 0.f;
	this->lastTime = 0.f;
	
	this->lastMouseX = 0.f;
	this->lastMouseY = 0.f;
	this->MouseX = 0.f;
	this->MouseY = 0.f;
	this->mouseOffsetX = 0.f;
	this->mouseOffsetY = 0.f;
	this->firstMouse = true;
	  	
	this->initGLFW();
	this->initWindow(title,resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initModels();
	this->initLights();
	this->initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window );
	glfwTerminate();
	for (size_t i = 0; i < this->shaders.size(); i++)
			delete this->shaders[i];
	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];
	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];
	for (auto*& i : this->models)
		delete i;
	for (size_t i = 0; i < this->lights.size(); i++)
		delete this->lights[i];
}
//Acessor
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Modifiers
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

//Functions
void Game::update()
{
	//Update Input---
	this->updateDT();
	this->updateInput();
	//for(size_t ii = 1; ii < this->models.size(); ii++)
	//{
	//	this->models[ii]->rotate(glm::vec3(0.f, 1.f, 0.f));
	//}
}

void Game::render()
{
	//DRAW---
	//Clear
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update uniforms
	this->updateUniforms();
	//render Models

	this->models[0]->render(this->shaders[SHADER_TERRAIN]);

	for (int ii = 1; ii <= this->models.size() - 1; ii++)
	{
		this->models[ii]->render(this->shaders[SHADER_CORE_PROGRAM]);
	}
	
	//End Draw
	glfwSwapBuffers(window);
	glFlush();
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::framebuffer_resize_callback(GLFWwindow * window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}