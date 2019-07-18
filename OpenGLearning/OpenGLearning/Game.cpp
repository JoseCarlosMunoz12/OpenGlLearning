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
		static_cast<float>(this->frameBufferWidth) / static_cast<float>(this->frameBufferHeight),
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
	this->textures.push_back(new Texture("Images/pusheen.png", GL_TEXTURE_2D,GL_RGBA ));
	this->textures.push_back(new Texture("Images/pusheen_specular.png", GL_TEXTURE_2D, GL_RGBA ));
	//Second Texture
	this->textures.push_back(new Texture("Images/container.png", GL_TEXTURE_2D, GL_RGBA ));
	this->textures.push_back(new Texture("Images/container_specular.png", GL_TEXTURE_2D, GL_RGBA ));
	//Third Image
	this->textures.push_back(new Texture("Images/Test1.png", GL_TEXTURE_2D, GL_RGBA ));
	this->textures.push_back(new Texture("Images/Test1_specular.png", GL_TEXTURE_2D, GL_RGBA ));
	//Blend Map Images
	this->textures.push_back(new Texture("Images/grassy2.png", GL_TEXTURE_2D, GL_RGBA ));
	this->textures.push_back(new Texture("Images/mud.png", GL_TEXTURE_2D, GL_RGBA ));
	this->textures.push_back(new Texture("Images/grassFlowers.png", GL_TEXTURE_2D, GL_RGBA ));
	this->textures.push_back(new Texture("Images/path.png", GL_TEXTURE_2D, GL_RGBA ));
	this->textures.push_back(new Texture("Images/blendMap.png", GL_TEXTURE_2D, GL_RGBA ));
	//Stall Texture
	this->textures.push_back(new Texture("Images/stallTexture.png", GL_TEXTURE_2D, GL_RGBA ));
	//Tree Texture
	this->textures.push_back(new Texture("Images/tree.png", GL_TEXTURE_2D, GL_RGBA ));
	//HeightMap Info
	this->MipMapsData.push_back(new MipMap("Images/heightMap.png", this->MapWidth,this->MapHeigth,10.f,500.f,500.f));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(SkyColor, glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0,1,"Material 1",0));
	this->materials.push_back(new Material(SkyColor, glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0,1, "Material 2",1));
	this->materials.push_back(new Material(SkyColor,{0,1,2,3,4,5},"Material 3",2));
}

void Game::initModels()
{
	meshes.push_back(
		new Mesh(
			&CustomTerrain(500, 500, this->MipMapsData[HEIGHTMAP_1]),
			"Terrain",
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));
	meshes.push_back(
		new Mesh(
			&CustomObject("Images/stall.obj"),
			"StallImage" + 0,
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(0.25f)));
	meshes.push_back(
		new Mesh(
			&CustomObject("Images/tree.obj"),
			"Tree",
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));
	this->models.push_back(new Model(
		glm::vec3(0.f, 0.f, 0.f),
		this->materials[TERRAIN_MAT],
		{ this->textures[6],this->textures[7], this->textures[8], this->textures[9],this->textures[10]},
		meshes[0],
		"Terrain"));
	this->NamesOfModels.push_back("Terrain");
	this->models.push_back(new Model(
		glm::vec3(9.f, this->MipMapsData[HEIGHTMAP_1]->ReturnValue(9.f, 0.f,RED_CHOSEN), 0.f),
		this->materials[MAT_1],
		{ this->textures[11],this->textures[11]},
		meshes[1],
		"Stall Image"));
	this->NamesOfModels.push_back("Stall Image");
	this->models.push_back(new Model(
		glm::vec3(0.f, this->MipMapsData[HEIGHTMAP_1]->ReturnValue(0.f, 9.f, RED_CHOSEN), 9.f),
		this->materials[MAT_2],
		{this->textures[12], this->textures[12]},
		meshes[2],
		"Tree"));
	this->NamesOfModels.push_back("Tree");
}

void Game::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 0.f, 0.f));
	this->TestLights.push_back(new Lights({ glm::vec3(0.f,0.f,0.f),glm::vec3(1.f,0.f,0.f) }));
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
		this->TestLights[0]->position =  glm::vec3(AmountZ, 0.f, Amount);
		AmountZ += 1;
	}
	if (glfwGetKey(this->window, GLFW_KEY_K) == GLFW_PRESS)
	{
		this->lights[0] = new glm::vec3(AmountZ, 0.f, Amount);
		this->TestLights[0]->position = glm::vec3(AmountZ, 0.f, Amount);
		AmountZ -= 1;
	}
	if (glfwGetKey(this->window, GLFW_KEY_I) == GLFW_PRESS)
	{
		this->lights[0] = new glm::vec3(AmountZ,0.f,Amount);
		this->TestLights[0]->position = glm::vec3(AmountZ, 0.f, Amount);
		Amount += 1;
	}
	if (glfwGetKey(this->window, GLFW_KEY_U) == GLFW_PRESS)
	{
		this->lights[0] = new glm::vec3(AmountZ, 0.f,Amount);
		this->TestLights[0]->position = glm::vec3(AmountZ, 0.f, Amount);
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
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->camera.move(this->dt, UP);
	}
	//None Camer inputs
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->MouseToUse.SetMouseCenter(this->window,this->Window_Width, this->Window_Height);
	}
}

void Game::updateMouseInput()
{
	this->MouseToUse.UpdateMouseInput(this->window);
	UICollision UICol;
	if (this->MakeMesh)
	{
		if (!UICol.ImGuiCollisionDetection(this->ScreenPos,this->WinSize,
											this->MouseToUse.getMousPos()))
		{
			if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
			{
				glm::vec3 RayRatio = this->MouseToUse.MouseRay({frameBufferWidth,frameBufferHeight},
										this->ProjectionMatrix,this->ViewMatrix);
				glm::vec3 MousePosition = this->camera.getPosition();
				this->SpaceLoc.x = -1 *( MousePosition.y * RayRatio.x /RayRatio.y - MousePosition.x );
				this->SpaceLoc.z = -1 *( MousePosition.y * RayRatio.z / RayRatio.y - MousePosition.z);
				this->SpaceLoc.y = this->MipMapsData[HEIGHTMAP_1]->ReturnValue(this->SpaceLoc.x,
																				this->SpaceLoc.z,
																						RED_CHOSEN);

			}
			static int oldState = GLFW_RELEASE;
			int newState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT);
			if (newState == GLFW_RELEASE && oldState == GLFW_PRESS)
			{
				if (this->NewTexId1 != -1)
				{
				this->CountMesh = this->models.size();
				std::string TempName = "Name" + std::to_string(this->CountMesh);
				this->NamesOfModels.push_back(TempName);
				this->models.push_back(new Model(
					this->SpaceLoc,
					this->NewMat,
					this->NewTex0[0],
					this->NewTex0[1],
					this->NewMesh,
					this->NamesOfModels[this->CountMesh].c_str()));
				this->NewTex0.clear();
				this->NewMatId = -1;
				this->NewTexId0 = -1;
				this->NewTexId1 = -1;
				this->NewMeshID = -1;
				}
			}
		oldState = newState;
		}
	}
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
	if (!this->MakeMesh)
	{
		this->camera.updateInput(dt, -1, this->MouseToUse.GetOffset());
	}
	
}

void Game::ImGuiOptions()
{
	ImGui::Begin("Added DifferentModels");
	this->ScreenPos.x = ImGui::GetWindowPos().x;
	this->ScreenPos.y = ImGui::GetWindowPos().y;
	this->WinSize.x = ImGui::GetWindowSize().x + ScreenPos.x + 7.f;
	this->WinSize.y = ImGui::GetWindowSize().y + ScreenPos.y + 7.f;
	//Info of each Model
	if (ImGui::TreeNode("Change Features of Chosen Model"))
	{
		int Count = 0;
		for (auto& ModelsFound : this->models)
		{
			if (ImGui::Selectable(ModelsFound->GetName(), this->ModelToMake == Count))
			{
				this->ModelToMake = Count;
			}
			Count++;
		}
		ImGui::Spacing();
		ImGui::Text("--Model Chosen--");
		if (this->ModelToMake != -1)
		{
			ImGui::Text(this->models[this->ModelToMake]->GetName());
			Material* ModMat = this->models[this->ModelToMake]->GetMaterial();
			glm::vec3 ModPos = this->models[this->ModelToMake]->GetPosition();
			std::vector<Texture*> ModTex = this->models[this->ModelToMake]->getTexture();
			std::vector<Mesh*> ModMesh = this->models[this->ModelToMake]->GetMeshes();
			ImGui::Text("Model Material Name = "); ImGui::SameLine(); ImGui::Text(ModMat->GetName());
			ImGui::Text("Model Position (X,Y,Z) ="); ImGui::SameLine(); ImGui::Text("(%f,%f,%f)", ModPos.x, ModPos.y, ModPos.z);
			ImGui::Text("Mesh used = "); ImGui::SameLine(); ImGui::Text(ModMesh[0]->GiveName());
			if (ImGui::TreeNode("Textures Used"))
			{
				for (auto& ii : ModTex)
				{
					ImGui::Text(ii->GiveChar());
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	ImGui::Spacing();
	ImGui::Spacing();
	//New Model Customization
	if (ImGui::TreeNode("---Change Info of New Model---"))
	{
		ImGui::Text("--Materials--");
		for (auto& MatCurrent : this->materials)
		{
			if (ImGui::Selectable(MatCurrent->GetName(),
				MatCurrent->GetMatId() == this->NewMatId))
			{
				this->NewMat = MatCurrent;
				this->NewMatId = MatCurrent->GetMatId();
			}
		}
		ImGui::Spacing();
		ImGui::Text("--Meshes--");
		int MeshCount = 0;
		for (auto& MeshCurrent : this->meshes)
		{
			if (ImGui::Selectable(MeshCurrent->GiveName(),
				MeshCount == NewMeshID))
			{
				this->NewMesh = MeshCurrent;
				this->NewMeshID = MeshCount;
			}
			MeshCount++;
		}
		ImGui::Spacing();
		ImGui::Text("--Textures to use--");
		int TexCount = 0;
		for (auto& TexCurrent : this->textures)
		{
			if (ImGui::Selectable(TexCurrent->GiveChar(),
				this->CheckNum(TexCount)))
			{
				if (this->NewTex0.size() != 2)
				{
					this->NewTex0.push_back(TexCurrent);
					if (this->NewTex0.size() == 1)
					{
						this->NewTexId0 = TexCount;
					}
					else
					{ 
						this->NewTexId1 = TexCount;
					}
				}
			}
			TexCount++;
		}
		ImGui::TreePop();
	}
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::End();
}

void Game::updateUniforms()
{
	//Update uniforms
	this->ViewMatrix = this->camera.GetViewMatrix();
	for (auto& ii :this->shaders)
	{
		ii->setVec3f(this->TestLights[0]->position, "lightPos0");
		ii->setVec3f(this->TestLights[0]->color, "lightColor");
		ii->setMat4fv(this->ViewMatrix, "ViewMatrix");
		ii->setVec3f(this->camera.getPosition(), "cameraPos");
	}
	//Update FramgeBuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov),
		static_cast<float>(this->frameBufferWidth) / static_cast<float>(this-> frameBufferHeight),
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
	if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		this->MakeMesh = false;
	}
	if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS)
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		this->MakeMesh = true;
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
	camera(glm::vec3(0.f,1.f,0.f),glm::vec3(0.f,0.f,1.f),glm::vec3(0.f,1.f,0.f)),
	rng(std::random_device()()),xDist(-100,100),yDist(-100,100)
{
	
	this->SkyColor = SkyColor;
	this->window = NULL;
	this->frameBufferHeight = this->Window_Height;
	this->frameBufferWidth = this->Window_Width;
		
	this-> camPosition = glm::vec3(0.f, 1.f, 0.f);
	this-> worldUp = glm::vec3(0.f, 1.f, 0.f);
	this-> camFront = glm::vec3(0.f, 0.f,-1.f);


	this-> fov = 90.f;
	this-> nearPlane = .1f;
	this-> farPlane = 1000.f;
	
	this->dt = 0.f;
	this->cuTime = 0.f;
	this->lastTime = 0.f;
		  	
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

bool Game::CheckNum(int Num)
{
	if (Num == this->NewTexId0)
	{
		return true;
	}
	else if (Num == this->NewTexId1)
	{
		return true;
	}
	else
	{
		return false;
	}
}