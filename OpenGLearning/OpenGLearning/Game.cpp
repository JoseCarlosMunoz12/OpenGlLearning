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
	
	glfwSetInputMode(this->window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
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
	this->shaders.push_back(new Shader(this->GLVerMajor, this->GLVerMinor,
		"MultiTexVertex.glsl", "MultiTexFragment.glsl"));
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
	//Blend Map Images
	this->textures.push_back(new Texture("Images/grassy2.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/mud.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/grassFlowers.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/path.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/blendMap.png", GL_TEXTURE_2D));
	//Stall Texture
	this->textures.push_back(new Texture("Images/stallTexture.png", GL_TEXTURE_2D));
	//Tree Texture
	this->textures.push_back(new Texture("Images/tree.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(SkyColor, glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0,1));
	this->materials.push_back(new Material(SkyColor, glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0,1));
	this->materials.push_back(new Material(SkyColor,{0,1,2,3,4}));
}

void Game::initModels()
{
	meshes.push_back(
		new Mesh(
			&CustomTerrain(800, 400),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));
	meshes.push_back(
		new Mesh(
			&CustomObject("Images/stall.obj"),
			glm::vec3(0.f, 0.f, -9.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(0.5f)));
	meshes.push_back(
		new Mesh(
			&CustomObject("Images/tree.obj"),
			glm::vec3(3.f, 0.f, 1.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));
	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[TERRAIN_MAT],
		{this->textures[6],this->textures[7], this->textures[8], this->textures[9],this->textures[10]},		
		meshes[0]));
	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[MAT_1],
		this->textures[11],
		this->textures[11],
		meshes[1]));
	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[MAT_2],
		this->textures[12],
		this->textures[12],
		meshes[2]));
}

void Game::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 0.f, 0.f));
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
	
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->MouseX, &this->MouseY);
	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		float mouse_current_z;
		glReadPixels(this->MouseX, this->MouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse_current_z);
		glm::vec3 windowCoordinates = glm::vec3(this->MouseX, this->MouseY, mouse_current_z);
		glm::vec4 viewport = glm::vec4(0.0f, 0.0f, (float)1920, (float)1080);
		this->worldCoordinates = glm::unProject(windowCoordinates, this->meshes[0]->ReturnModelMatrix(), this->ProjectionMatrix, viewport);

	}
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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	this->updateOpenGLOptions();
	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt,-1,this->mouseOffsetX, this->mouseOffsetY);
}

void Game::ImGuiOptions()
{
	{
		ImGuiIO& io = ImGui::GetIO();
		static float XPosition, YPosition;
		ImGui::Begin("Hello, world!");
		ImGui::SliderFloat("X position of the Tree",&XPosition,-40,40);
		ImGui::SliderFloat("Y position of the Tree",&YPosition,-40,40); 
		ImGui::Text("%g,%g",io.MousePos.x, io.MousePos.y);
		ImGui::Text("%f,%f,%f",worldCoordinates.x, worldCoordinates.y, worldCoordinates.z );
		ImGui::Text("Mouse down:");
		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) 
			if (io.MouseDownDuration[i] >= 0.0f)
			{ 
				ImGui::SameLine();
				ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
			}
		if (ImGui::Button("Create Trees Randomly"))
		{
			if (meshes.size() < 2)
				{
					meshes.push_back(
						new Mesh(&CustomObject("Images/tree.obj"),
							glm::vec3(0.f),
							glm::vec3(0.f),
							glm::vec3(0.f),
							glm::vec3(1.f)));
				}
			this->models.push_back(new Model(
				glm::vec3(XPosition, 0.f, YPosition),
				this->materials[0],
				this->textures[12],
				this->textures[12],
				meshes[2]));
		}
		ImGui::Text("%d",models.size() - 2);
		if (ImGui::Button("Destroy Last Tree Created"))
		{
			if (this->models.size() > 3)
			{
				models.pop_back();
			}
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

void Game::updateUniforms()
{
	//Update uniforms
	this->ViewMatrix = this->camera.GetViewMatrix();
	for (auto& ii :this->shaders)
	{
		ii->setVec3f(*this->lights[0], "lightPos0");
		ii->setMat4fv(this->ViewMatrix, "ViewMatrix");
		ii->setVec3f(this->camera.getPosition(), "cameraPos");
	}
	//Update FramgeBuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov),
		static_cast<float>(this->frameBufferWidth) / this-> frameBufferHeight,
		this->nearPlane,
		this->farPlane);
	for (auto& ii : this->shaders)
	{
		ii->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
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
	const int GLmajorVer, const int GLminorVer, bool resizable,glm::vec3 SkyColor)
	: Window_Width(width), Window_Height(height),
	GLVerMajor(GLmajorVer), GLVerMinor(GLminorVer),
	camera(glm::vec3(0.f,1.f,1.f),glm::vec3(0.f,0.f,1.f),glm::vec3(0.f,1.f,0.f)),
	rng(std::random_device()()),xDist(-100,100),yDist(-100,100)
{
	
	this->SkyColor = SkyColor;
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

	const char* glsl_version = "#version 130";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

Game::~Game()
{
	ImGui::DestroyContext();
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
	for (auto& i : meshes)
		delete i;
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
	this->ImGuiOptions();
}

void Game::render()
{
	//DRAW---
	//Clear
	ImGui::Render();
	glClearColor(0.f,0.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	//Update uniforms
	this->updateUniforms();
	//render Models

	this->models[0]->renderManyTextures(this->shaders[SHADERS_TERRAIN]);
	for (size_t ii = 1; ii < models.size(); ii++)
	{
		this->models[ii]->render(this->shaders[SHADER_CORE_PROGRAM]);
	}
		
	//End Draw

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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