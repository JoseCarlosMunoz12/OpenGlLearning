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
	//Regular Shaders to Draw
	this->shaders.push_back(new Shader(0,this->GLVerMajor , this->GLVerMinor ,
		"Shaders/vertex_core.glsl", "Shaders/fragment_core.glsl"));
	this->shaders.push_back(new Shader(1,this->GLVerMajor, this->GLVerMinor,
		"Shaders/terrain_vertex.glsl", "Shaders/terrain_fragment.glsl"));
	this->shaders.push_back(new Shader(2,this->GLVerMajor, this->GLVerMinor,
		"Shaders/MultiTexVertex.glsl", "Shaders/MultiTexFragment.glsl"));
	//Shadow Shaders
	this->shaders.push_back(new Shader(3, this->GLVerMajor, this->GLVerMinor,
		"Shaders/ShadowMapVertex.glsl", "Shaders/ShadowMapFrag.glsl"));
	//Shadow Debug information
	this->shaders.push_back(new Shader(4, this->GLVerMajor, this->GLVerMinor,
		"Shaders/DebugVertex.glsl", "Shaders/DebugFrag.glsl"));
}

void Game::initShadows()
{
	this->Shadows.push_back(new ShadowTex("Shadow0"));
	this->Shadows.push_back(new ShadowTex("Shadow1"));
	this->Shadows.push_back(new ShadowTex("Shadow2"));
	for (auto& ii : this->Shadows)
	{
		ii->Init(this->Window_Width, this->Window_Height);
	}
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
	//Shadow MapCreation
	for (auto& ii : this->Shadows)
	{
		this->textures.push_back(ii);
	}	
	//HeightMap Info
	this->MipMapsData.push_back(new MipMap("Images/heightmap.png", this->MapWidth,this->MapHeigth,10.f,500.f,500.f));
}

void Game::initMaterials()
{
	//Testbug the new mats	
	this->MatTest.push_back(new MipMapMat("TerrainMat", 0, MAT_2,
		this->SkyColor, { 0,1,2,3,4 }, { 5,6 }, {}, {7}));
	this->MatTest.push_back(new TxtMat("Model Terrain", 1, MAT_0,
							this->SkyColor, glm::vec3(0.1f),
							glm::vec3(1.f), glm::vec3(1.f),
		0, 1, { 2,3 }, {}, {4}));
	this->MatTest.push_back(new TxtMat("Model File", 2, MAT_1,
							this->SkyColor, glm::vec3(0.1f),
							glm::vec3(1.f), glm::vec3(1.f),
		0, 1, { 2, 3}, {}, {4}));
	this->MatTest.push_back(new SingleTextMat("Single", 3, 4,
								this->SkyColor, 0));
}

void Game::initModels()
{
	//Meshes Made to be use
	meshes.push_back(
		new Mesh(
			&CustomTerrain(500, 500, this->MipMapsData[HEIGHTMAP_1]),
			"Terrain"));
	meshes.push_back(
		new Mesh(
			&CustomObject("Images/stall.obj"),
			"StallImage" + 0));
	meshes.push_back(
		new Mesh(
			&PlaneTerrain(),
			"Tree"));
	meshes.push_back(new Mesh(
		&Quad(),
		"Tree1"));
	meshes.push_back(
		new Mesh(&Sphere(1, 32, 32),
			"Sphere"));
	meshes.push_back(
		new Mesh(&Cylinder(1.f, 4.f, 40.f),
			"Cylinder"));
	meshes.push_back(
		new Mesh(&Cone(1.f, 1.f, 40.f, 10),
			"Cone"));
	meshes.push_back(
		new Mesh(&Cube(),
			"Cube"));

	//
	//Meshes Componets are made
	//
	MeshsArtifacts Terrain(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f),
		0, 0, { 0,1,2,3,4,5,6,7 });
	MeshsArtifacts Stalls(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f),
		0, 0, { 0,1,2,3,4 });
	MeshsArtifacts Flat(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f),
		0, 0, { 0,1,2,3 });
	MeshsArtifacts Plane(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f),
		0, 0, {0});
	std::vector<MeshsArtifacts> HierArch1;
	HierArch1.push_back(MeshsArtifacts(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f,90.f,0.f), glm::vec3(.1f),
		0, 0, { 0,1,2,3,4}));
	HierArch1.push_back(MeshsArtifacts(glm::vec3(1.f, 0.f, 1.f), glm::vec3(1.f, 0.f, 1.f), glm::vec3(0.f), glm::vec3(1.f),
		1, 0, { 0,1,2,3,4 }));
	HierArch1.push_back(MeshsArtifacts(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f), glm::vec3(1.f),
		2, 1, { 0,1,2,3,4 }));
	HierArch1.push_back(MeshsArtifacts(glm::vec3(10.f, 0.f, 10.f), glm::vec3(10.f, 0.f, 10.f), glm::vec3(0.f), glm::vec3(1.f),
		2, 2, { 1,0,2,3,4 }));
	//
	//Pushing Models
	//
	this->models.push_back(new Model("Terrain",
		glm::vec3(0.f, 0.f, 0.f),
		this->MatTest[0],
		{ this->textures[6],this->textures[7], this->textures[8], this->textures[9],this->textures[10],
		this->textures[13] ,this->textures[14],this->textures[15] },
		meshes[0], { Terrain }));
	this->NamesOfModels.push_back("Terrain");
	this->models.push_back(new Model("Stall Image1",
		glm::vec3(0.f, this->MipMapsData[HEIGHTMAP_1]->ReturnValue(0.f, 9.f), 9.f),
		this->MatTest[1], { this->textures[10],this->textures[11],this->textures[13],this->textures[14],this->textures[15] },
		meshes[1], { Stalls }));
	this->NamesOfModels.push_back("Stall Image1");
	this->models.push_back(new Model("Stall Image2",
		glm::vec3(9.f, this->MipMapsData[HEIGHTMAP_1]->ReturnValue(9.f, 0.f), 0.f),
		this->MatTest[1], { this->textures[2],this->textures[3] ,this->textures[13],this->textures[14],this->textures[15] },
		meshes[1], { Flat }));

	this->NamesOfModels.push_back("Stall Image2");
	this->models.push_back(new Model("Face R",
		glm::vec3(0.f, this->MipMapsData[HEIGHTMAP_1]->ReturnValue(0.f, 1.f), 1.f),
		this->MatTest[2], { this->textures[11],this->textures[11],this->textures[13],this->textures[14],this->textures[15] },
		{ meshes[6],meshes[2],meshes[1] }, HierArch1));
	this->NamesOfModels.push_back("Face R");

	this->models.push_back(new Model("DebugImage",
		glm::vec3(0.f, this->MipMapsData[HEIGHTMAP_1]->ReturnValue(0.f, -2.f) +1.f, -2.f),
		this->MatTest[3], { this->textures[13] },
		{ meshes[3] }, {Plane}));
	this->NamesOfModels.push_back("Debug Image");
	this->models.push_back(new Model("DebugImage1",
		glm::vec3(1.f, this->MipMapsData[HEIGHTMAP_1]->ReturnValue(1.f, -2.f) + 1.f, -2.f),
		this->MatTest[3], { this->textures[15] },
		{ meshes[3] }, { Plane }));
	this->NamesOfModels.push_back("Debug Image1");
}

void Game::initLights()
{
	//Initializing Dir lights
	this->DirLights.push_back(new DrLights(0,glm::vec3(-1.f, this->MipMapsData[0]->ReturnValue(-1.f, -1.f) + 5.f, -1.f),
		glm::vec3(1.f, 1.f, 1.f), this->frameBufferWidth, this->frameBufferWidth));
	this->DirLights.push_back(new DrLights(1, glm::vec3(-1.f, this->MipMapsData[0]->ReturnValue(-1.f, -1.f) + 5.f, -1.f),
		glm::vec3(1.f, 0.f, 1.f), this->frameBufferWidth, this->frameBufferWidth));
	//Init Cone Lights
	//Init Arealights
	this->ArLights.push_back(new AreaLights(glm::vec3(1.f, 1.f, 1.f),
		glm::vec3(-1.f, this->MipMapsData[0]->ReturnValue(-1.f, 1.f) + 5.f, 1.f),
		24.f,12.f,0));

	for (auto& ii : this->DirLights)
	{
		this->LightsToUse.push_back(ii);
	}
	for (auto& ii : this->CnLights)
	{
		this->LightsToUse.push_back(ii);
	}
	for (auto& ii : this-> ArLights)
	{
		this->LightsToUse.push_back(ii);
	}

}

void Game::initUniforms()
{
	for (auto& ii : this->shaders)
	{
		ii->setMat4fv(ViewMatrix, "ViewMatrix");
		ii->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
		int Value = 0;
		for (auto& jj : this->DirLights)
		{
			std::string LightPos = "AllLightInf[" + std::to_string(Value) + "].LightPos";
			std::string LightClr = "AllLightInf[" + std::to_string(Value) + "].LightColor";
			ii->setVec3f(jj->GetPos(), LightPos.c_str());
			ii->setVec3f(jj->GetColor(), LightClr.c_str());
			Value++;
		}
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
		this->LightsToUse[0]->Move( glm::vec3(1.f, 0.f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_K) == GLFW_PRESS)
	{
		this->LightsToUse[0]->Move(glm::vec3(-1.f, 0.f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_I) == GLFW_PRESS)
	{
		this->LightsToUse[0]->Move(glm::vec3(0.f, 0.f, 1.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_U) == GLFW_PRESS)
	{
		this->LightsToUse[0]->Move(glm::vec3(0.f, 0.f, -1.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS)
	{
		this->LightsToUse[0]->Move(glm::vec3(0.f, -1.f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS)
	{
		this->LightsToUse[0]->Move(glm::vec3(0.f, 1.f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		this->LightsToUse[0]->SetPosition(glm::vec3(0.f,this->MipMapsData[0]->ReturnValue(0.f,0.f), 0.f));
	}
	//End Game
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
	//None Camera inputs
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->MouseToUse.SetMouseCenter(this->window,this->Window_Width, this->Window_Height);
	}
}

void Game::updateMouseInput()
{
	this->MouseToUse.UpdateMouseInput(this->window);
	if (this->MakeMesh)
	{
		if (!this->MouseToUse.UpdateMouse2dInput(this->window, {this->ScreenPos,this->WinSize}))
		{

			if (this->MouseToUse.MouseButtonClicked(this->window, GLFW_MOUSE_BUTTON_1))
			{
				this->SpaceLoc = this->MouseToUse.NewPosition(this->MipMapsData[0],
					{frameBufferWidth,frameBufferHeight },
					this->ProjectionMatrix,
					this->ViewMatrix,
					this->camera.getPosition());
			}
			if (this->MouseToUse.MouseButtonClicked(this->window, GLFW_MOUSE_BUTTON_2))
			{

			}
		}
	}
	else
	{
		this->camera.updateInput(dt, -1, this->MouseToUse.GetOffset());
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
}

void Game::ImGuiOptions()
{
	glm::vec3 TempCamera = this->camera.getPosition();
	ImGui::Begin("Added DifferentModels");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Spacing();
	ImGui::Text("Cam Position (X,Y,Z) ="); ImGui::SameLine(); ImGui::Text("(%f,%f,%f)", TempCamera.x, TempCamera.y, TempCamera.z);
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
			StdMat* ModMat = this->models[this->ModelToMake]->GetStdMat();
			glm::vec3 ModPos = this->models[this->ModelToMake]->GetPosition();
			std::vector<GeneralTextInfo*> ModTex = this->models[this->ModelToMake]->getTexture();
			std::vector<Nodes*> TempNodes = this->models[this->ModelToMake]->GetNodesInfo();
			std::vector<Mesh*> ModMesh = this->models[this->ModelToMake]->GetMeshes();
			ImGui::Text("Model Material Name = "); ImGui::SameLine(); ImGui::Text(ModMat->GetName());
			ImGui::Text("Model Position (X,Y,Z) ="); ImGui::SameLine(); ImGui::Text("(%f,%f,%f)", ModPos.x, ModPos.y, ModPos.z);
			for (auto& ii : ModMesh)
			{
				ImGui::Text("Mesh used = "); ImGui::SameLine(); ImGui::Text(ii->GiveName());
			}
			if (ImGui::TreeNode("Textures Used"))
			{
				for (auto& ii : ModTex)
				{
					ImGui::Text(ii->GiveChar());
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Nodes Info"))
			{
				if (ImGui::TreeNode("Position"))
				{
					int CountOfPos = 0;
					for (auto& ii : TempNodes)
					{
						glm::vec3 TempVec = ii->GetPosition();
						ImGui::Text("Node %d Pos =",CountOfPos); ImGui::SameLine(); ImGui::Text("(%f,%f,%f)", TempVec.x, TempVec.y, TempVec.z);
						CountOfPos++;
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Rotation"))
				{
					int CountOfRot = 0;
					for (auto& ii : TempNodes)
					{
						glm::vec3 TempVec = ii->GetRotation();
						ImGui::Text("Node %d Rotation =",CountOfRot); ImGui::SameLine(); ImGui::Text("(%f,%f,%f)", TempVec.x, TempVec.y, TempVec.z);
						CountOfRot++;
					}
					ImGui::TreePop();
				}				
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	ImGui::Spacing();
	ImGui::Spacing();
	//New Model Customization
	if (ImGui::TreeNode("PreMade Primtives, Meshes, etc."))
	{
		if(ImGui::TreeNode("Shaders"))
		{
			for (auto& ii : this->shaders)
			{
				if (ImGui::Selectable(ii->GetName()))
				{
					std::cout << ii->GetShaderId() << "\n ";
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Materials"))
		{
			for (auto& ii : this->MatTest)
			{
				if (ImGui::Selectable(ii->GetName()))
				{
					std::cout << ii->GetShaderId() << "\n ";
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Textures"))
		{
			for (auto& ii : this->textures)
			{
				if (ImGui::Selectable(ii->GiveChar()))
				{
					std::cout << ii->GiveChar() << "\n ";
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Meshes"))
		{
			for (auto& ii : this->meshes)
			{
				if (ImGui::Selectable(ii->GiveName()))
				{
					std::cout << ii->GiveName() << "\n";
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::Spacing();
	ImGui::Spacing();
	if (ImGui::TreeNode("Lights Information"))
	{ 
		if (ImGui::TreeNode("Directional Lights")) {
			int Count = 0;
			for (auto& ii : this->DirLights)
			{
				std::string LightPos = "Dir Lights Num-" + std::to_string(Count);
				if (ImGui::Selectable(LightPos.c_str(), this->DirLightsToShow  == Count))
				{
					this->DirLightsToShow  = Count;
				}
				Count++;
			}
			if (this->DirLightsToShow  != -1)
			{
				glm::vec3 ColPos = this->DirLights[this->DirLightsToShow ]->GetPos();
				glm::vec3 Col = this->DirLights[this->DirLightsToShow ]->GetColor();
				float TempYaw = this->DirLights[this->DirLightsToShow ]->GetYaw();
				float TempPitch = this->DirLights[this->DirLightsToShow ]->GetPitch();
				OrthoView Tempview = this->DirLights[this->DirLightsToShow ]->GetOrthoParts();
				glm::vec3 LightAmbient = this->DirLights[this->DirLightsToShow ]->GetAmbient();
				glm::vec3 LightDiffuse = this->DirLights[this->DirLightsToShow ]->GetDiffuse();
				glm::vec3 LightSpecular = this->DirLights[this->DirLightsToShow ]->GetSpecular();
				//Position of the Light
				ImGui::Text("Light Position (%f,%f,%f)", ColPos.x, ColPos.y, ColPos.z);
				if (ImGui::SliderFloat("X Position", &ColPos.x, -10.f, 10.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetPosition(ColPos);
				}
				if (ImGui::SliderFloat("Y Position", &ColPos.y, 0, 20.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetPosition(ColPos);
				}
				if (ImGui::SliderFloat("Z Position", &ColPos.z, -40.f, 40.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetPosition(ColPos);
				}
				//View of the Light
				ImGui::Text(" Light Yaw and Pitch (%f,%f)", TempYaw, TempPitch);
				if (ImGui::SliderFloat("Light Yaw", &TempYaw, -180.f, 180.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetYaw(TempYaw);
				}
				if (ImGui::SliderFloat("Light Pitch", &TempPitch, -90.f, 0.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetPitch(TempPitch);
				}
				float Cols[3];
				Cols[0] = Col.r;
				Cols[1] = Col.g;
				Cols[2] = Col.b;
				//Color Components
				ImGui::Text("Light Components");
				if (ImGui::TreeNode("Ambient"))
				{
					if (ImGui::SliderFloat("X Component", &LightAmbient.x, 0.f, 1.0f))
					{
						this->DirLights[this->DirLightsToShow ]->SetAmbient(LightAmbient);
					}
					if (ImGui::SliderFloat("Y Component", &LightAmbient.y, 0.f, 1.0f))
					{
						this->DirLights[this->DirLightsToShow ]->SetAmbient(LightAmbient);
					}
					if (ImGui::SliderFloat("Z Component", &LightAmbient.z, 0.f, 1.0f))
					{
						this->DirLights[this->DirLightsToShow ]->SetAmbient(LightAmbient);
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Diffuse"))
				{
					if (ImGui::SliderFloat("X Component", &LightDiffuse.x, 0.f, 1.0f))
					{
						this->DirLights[this->DirLightsToShow ]->SetDiffuse(LightDiffuse);
					}
					if (ImGui::SliderFloat("Y Component", &LightDiffuse.y, 0.f, 1.0f))
					{
						this->DirLights[this->DirLightsToShow ]->SetDiffuse(LightDiffuse);
					}
					if (ImGui::SliderFloat("Z Component", &LightDiffuse.z, 0.f, 1.0f))
					{
						this->DirLights[this->DirLightsToShow ]->SetDiffuse(LightDiffuse);
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Specular"))
				{
					if (ImGui::SliderFloat("X Component", &LightSpecular.x, 0.f, 1.0f))
					{
						this->DirLights[this->DirLightsToShow ]->SetSpecular(LightSpecular);
					}
					if (ImGui::SliderFloat("Y Component", &LightSpecular.y, 0.f, 1.0f))
					{
						this->DirLights[this->DirLightsToShow ]->SetSpecular(LightSpecular);
					}
					if (ImGui::SliderFloat("Z Component", &LightSpecular.z, 0.f, 1.0f))
					{
						this->DirLights[this->DirLightsToShow ]->SetSpecular(LightSpecular);
					}
					ImGui::TreePop();
				}
				//Color of Light
				ImGui::Text("Color Information");
				if (ImGui::ColorEdit3("Color Wheel", Cols))
				{
					this->DirLights[this->DirLightsToShow ]->SetColor(glm::vec3(Cols[0], Cols[1], Cols[2]));
				}
				float Views[5];
				Views[0] = Tempview.Left;
				Views[1] = Tempview.Right;
				Views[2] = Tempview.Bottom;
				Views[3] = Tempview.Up;
				Views[4] = Tempview.FarPlane;
				//Position of the Light
				ImGui::Text("Ortho view information");
				if (ImGui::SliderFloat("Left", &Views[0], -30.f, -10.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetOrthoParts({ Views[0],Views[1], Views[2], Views[3], Views[4] });
				}
				if (ImGui::SliderFloat("Right", &Views[1], 10.f, 30.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetOrthoParts({ Views[0],Views[1], Views[2], Views[3], Views[4] });
				}
				if (ImGui::SliderFloat("Bottom", &Views[2], -30.f, -10.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetOrthoParts({ Views[0],Views[1], Views[2], Views[3], Views[4] });
				}
				if (ImGui::SliderFloat("Up", &Views[3], 10.f, 30.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetOrthoParts({ Views[0],Views[1], Views[2], Views[3], Views[4] });
				}
				if (ImGui::SliderFloat("Far Plane", &Views[4], 10.f, 30.f))
				{
					this->DirLights[this->DirLightsToShow ]->SetOrthoParts({ Views[0],Views[1], Views[2], Views[3], Views[4] });
				}

			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Cone Lights"))
		{
			int Count = 0;
			for (auto& ii : this->CnLights)
			{
				std::string CnLightName = "Cn Light Num-" + std::to_string(Count);
				if (ImGui::Selectable(CnLightName.c_str(), this->CnLightsToShow == Count))
				{
					this->CnLightsToShow = Count;
				}
				Count++;
			}
			if (this->CnLightsToShow != -1)
			{
				glm::vec3 ColPos = this->CnLights[this->CnLightsToShow]->GetPos();
				glm::vec3 Col = this->CnLights[this->CnLightsToShow]->GetColor();
				float TempYaw = this->CnLights[this->CnLightsToShow]->GetYaw();
				float TempPitch = this->CnLights[this->CnLightsToShow]->GetPitch();
				glm::vec3 LightAmbient = this->CnLights[this->CnLightsToShow]->GetAmbient();
				glm::vec3 LightDiffuse = this->CnLights[this->CnLightsToShow]->GetDiffuse();
				glm::vec3 LightSpecular = this->CnLights[this->CnLightsToShow]->GetSpecular();
				float LightCn = this->CnLights[this->CnLightsToShow]->GetCone();
				ImGui::Text("Light Position (%f,%f,%f)", ColPos.x, ColPos.y, ColPos.z);
				if (ImGui::SliderFloat("X Position", &ColPos.x, -10.f, 10.f))
				{
					this->CnLights[this->CnLightsToShow]->SetPosition(ColPos);
				}
				if (ImGui::SliderFloat("Y Position", &ColPos.y, 0, 20.f))
				{
					this->CnLights[this->CnLightsToShow]->SetPosition(ColPos);
				}
				if (ImGui::SliderFloat("Z Position", &ColPos.z, -40.f, 40.f))
				{
					this->CnLights[this->CnLightsToShow]->SetPosition(ColPos);
				}
				//View of the Light
				ImGui::Text(" Light Yaw and Pitch (%f,%f)", TempYaw, TempPitch);
				if (ImGui::SliderFloat("Light Yaw", &TempYaw, -180.f, 180.f))
				{
					this->CnLights[this->CnLightsToShow]->SetYaw(TempYaw);
				}
				if (ImGui::SliderFloat("Light Pitch", &TempPitch, -90.f, 0.f))
				{
					this->CnLights[this->CnLightsToShow]->SetPitch(TempPitch);
				}
				ImGui::Text("Light Cone Max");
				if (ImGui::SliderFloat("",&LightCn,0.f,90.f))
				{
					this->CnLights[this->CnLightsToShow]->SetConeAngle(LightCn);
				}
				float Cols[3];
				Cols[0] = Col.r;
				Cols[1] = Col.g;
				Cols[2] = Col.b;
				//Color Components
				ImGui::Text("Light Components");
				if (ImGui::TreeNode("Ambient"))
				{
					if (ImGui::SliderFloat("X Component", &LightAmbient.x, 0.f, 1.0f))
					{
						this->CnLights[this->CnLightsToShow]->SetAmbient(LightAmbient);
					}
					if (ImGui::SliderFloat("Y Component", &LightAmbient.y, 0.f, 1.0f))
					{
						this->CnLights[this->CnLightsToShow]->SetAmbient(LightAmbient);
					}
					if (ImGui::SliderFloat("Z Component", &LightAmbient.z, 0.f, 1.0f))
					{
						this->CnLights[this->CnLightsToShow]->SetAmbient(LightAmbient);
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Diffuse"))
				{
					if (ImGui::SliderFloat("X Component", &LightDiffuse.x, 0.f, 1.0f))
					{
						this->CnLights[this->CnLightsToShow]->SetDiffuse(LightDiffuse);
					}
					if (ImGui::SliderFloat("Y Component", &LightDiffuse.y, 0.f, 1.0f))
					{
						this->CnLights[this->CnLightsToShow]->SetDiffuse(LightDiffuse);
					}
					if (ImGui::SliderFloat("Z Component", &LightDiffuse.z, 0.f, 1.0f))
					{
						this->CnLights[this->CnLightsToShow]->SetDiffuse(LightDiffuse);
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Specular"))
				{
					if (ImGui::SliderFloat("X Component", &LightSpecular.x, 0.f, 1.0f))
					{
						this->CnLights[this->CnLightsToShow]->SetSpecular(LightSpecular);
					}
					if (ImGui::SliderFloat("Y Component", &LightSpecular.y, 0.f, 1.0f))
					{
						this->CnLights[this->CnLightsToShow]->SetSpecular(LightSpecular);
					}
					if (ImGui::SliderFloat("Z Component", &LightSpecular.z, 0.f, 1.0f))
					{
						this->CnLights[this->CnLightsToShow]->SetSpecular(LightSpecular);
					}
					ImGui::TreePop();
				}
				//Color of Light
				ImGui::Text("Color Information");
				if (ImGui::ColorEdit3("Color Wheel", Cols))
				{
					this->CnLights[this->CnLightsToShow]->SetColor(glm::vec3(Cols[0], Cols[1], Cols[2]));
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Point Lights"))
		{
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Area Lights"))
		{
			int Count = 0;
			for (auto& ii : this->ArLights)
			{
				std::string ArLightName = "Ar Light Num-" + std::to_string(Count);
				if (ImGui::Selectable(ArLightName.c_str(), this->ArLightsToShow == Count))
				{
					this->ArLightsToShow = Count;
				}
				Count++;
			}
			if (this->ArLightsToShow != -1)
			{
				glm::vec3 ColPos = this->ArLights[this->ArLightsToShow]->GetPos();
				glm::vec3 Col = this->ArLights[this->ArLightsToShow]->GetColor();
				float TempYaw = this->ArLights[this->ArLightsToShow]->GetYaw();
				float TempPitch = this->ArLights[this->ArLightsToShow]->GetPitch();
				glm::vec3 LightAmbient = this->ArLights[this->ArLightsToShow]->GetAmbient();
				glm::vec3 LightDiffuse = this->ArLights[this->ArLightsToShow]->GetDiffuse();
				glm::vec3 LightSpecular = this->ArLights[this->ArLightsToShow]->GetSpecular();
				float LightCon = this->ArLights[this->ArLightsToShow]->GetCone();
				float LightUm = this->ArLights[this->ArLightsToShow]->GetUmbra();
				float LightConst = this->ArLights[this->ArLightsToShow]->GetConstant();
				float LightLin = this->ArLights[this->ArLightsToShow]->GetLinear();
				float LightQuad = this->ArLights[this->ArLightsToShow]->GetQuad();
				ImGui::Text("Light Position (%f,%f,%f)", ColPos.x, ColPos.y, ColPos.z);
				if (ImGui::SliderFloat("X Position", &ColPos.x, -10.f, 10.f))
				{
					this->ArLights[this->ArLightsToShow]->SetPosition(ColPos);
				}
				if (ImGui::SliderFloat("Y Position", &ColPos.y, 0, 20.f))
				{
					this->ArLights[this->ArLightsToShow]->SetPosition(ColPos);
				}
				if (ImGui::SliderFloat("Z Position", &ColPos.z, -40.f, 40.f))
				{
					this->ArLights[this->ArLightsToShow]->SetPosition(ColPos);
				}
				//View of the Light
				ImGui::Text(" Light Yaw and Pitch (%f,%f)", TempYaw, TempPitch);
				if (ImGui::SliderFloat("Light Yaw", &TempYaw, -180.f, 180.f))
				{
					this->ArLights[this->ArLightsToShow]->SetYaw(TempYaw);
				}
				if (ImGui::SliderFloat("Light Pitch", &TempPitch, -90.f, 0.f))
				{
					this->ArLights[this->ArLightsToShow]->SetPitch(TempPitch);
				}
				ImGui::Text("Light Cone Max");
				if (ImGui::SliderFloat("", &LightCon, 0.f, 90.f))
				{
					this->ArLights[this->ArLightsToShow]->SetConeAngle(LightCon);
				}
				ImGui::Text("Light Umbra Max");
				if (ImGui::SliderFloat("", &LightUm, 0.f, 90.f))
				{
					this->ArLights[this->ArLightsToShow]->SetUmbraAngle(LightUm);
				}
				float Cols[3];
				Cols[0] = Col.r;
				Cols[1] = Col.g;
				Cols[2] = Col.b;
				//Color Components
				ImGui::Text("Light Components");
				if (ImGui::TreeNode("Ambient"))
				{
					if (ImGui::SliderFloat("X Component", &LightAmbient.x, 0.f, 1.0f))
					{
						this->ArLights[this->ArLightsToShow]->SetAmbient(LightAmbient);
					}
					if (ImGui::SliderFloat("Y Component", &LightAmbient.y, 0.f, 1.0f))
					{
						this->ArLights[this->ArLightsToShow]->SetAmbient(LightAmbient);
					}
					if (ImGui::SliderFloat("Z Component", &LightAmbient.z, 0.f, 1.0f))
					{
						this->ArLights[this->ArLightsToShow]->SetAmbient(LightAmbient);
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Diffuse"))
				{
					if (ImGui::SliderFloat("X Component", &LightDiffuse.x, 0.f, 1.0f))
					{
						this->ArLights[this->ArLightsToShow]->SetDiffuse(LightDiffuse);
					}
					if (ImGui::SliderFloat("Y Component", &LightDiffuse.y, 0.f, 1.0f))
					{
						this->ArLights[this->ArLightsToShow]->SetDiffuse(LightDiffuse);
					}
					if (ImGui::SliderFloat("Z Component", &LightDiffuse.z, 0.f, 1.0f))
					{
						this->ArLights[this->ArLightsToShow]->SetDiffuse(LightDiffuse);
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Specular"))
				{
					if (ImGui::SliderFloat("X Component", &LightSpecular.x, 0.f, 1.0f))
					{
						this->ArLights[this->ArLightsToShow]->SetSpecular(LightSpecular);
					}
					if (ImGui::SliderFloat("Y Component", &LightSpecular.y, 0.f, 1.0f))
					{
						this->ArLights[this->ArLightsToShow]->SetSpecular(LightSpecular);
					}
					if (ImGui::SliderFloat("Z Component", &LightSpecular.z, 0.f, 1.0f))
					{
						this->ArLights[this->ArLightsToShow]->SetSpecular(LightSpecular);
					}
					ImGui::TreePop();
				}
				//Color of Light
				ImGui::Text("Color Information");
				if (ImGui::ColorEdit3("Color Wheel", Cols))
				{
					this->ArLights[this->ArLightsToShow]->SetColor(glm::vec3(Cols[0], Cols[1], Cols[2]));
				}

			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void Game::updateUniforms()
{
	//Update uniforms
	this->ViewMatrix = this->camera.GetViewMatrix();

	int DirLightCount = this->DirLights.size();
	int CnLightCount = this->CnLights.size();
	int ArLightCount = this->ArLights.size();
	for (auto& ii :this->shaders)
	{ 
		ii->set1i(DirLightCount, "DirLightCount");
		ii->set1i(CnLightCount, "CnLightCount");
		ii->set1i(ArLightCount,"ArLightCount");
		int Value = 0;
		for (auto& jj: this->DirLights)
		{
			//Standard information
			std::string LightPos = "AllDirInfo[" + std::to_string(Value) + "].LightPos";
			std::string LightClr = "AllDirInfo[" + std::to_string(Value) + "].LightColor";
			ii->setVec3f(jj->GetPos(),  LightPos.c_str());
			ii->setVec3f(jj->GetColor(), LightClr.c_str());
			//Light Prop Info
			std::string LightAmbient = "AllDirInfo[" + std::to_string(Value) + "].Ambient";
			std::string LightDiffuse = "AllDirInfo[" + std::to_string(Value) + "].Diffuse";
			std::string LightSpecular = "AllDirInfo[" + std::to_string(Value) + "].Specular";
			ii->setVec3f(jj->GetAmbient(), LightAmbient.c_str());
			ii->setVec3f(jj->GetDiffuse(), LightDiffuse.c_str());
			ii->setVec3f(jj->GetSpecular(), LightSpecular.c_str());
			Value++;
		}
		Value = 0;
		for (auto& jj : this->CnLights)
		{
			std::string LightPos = "AllCnInfo[" + std::to_string(Value) + "].LightPos";
			std::string LightClr = "AllCnInfo[" + std::to_string(Value) + "].LightColor";
			std::string LightDir = "AllCnInfo[" + std::to_string(Value) + "].LightDirection";
			ii->setVec3f(jj->GetPos(), LightPos.c_str());
			ii->setVec3f(jj->GetColor(), LightClr.c_str());
			ii->setVec3f(jj->GetFront(), LightDir.c_str());
			//Light Prop Info
			std::string LightAmbient = "AllCnInfo[" + std::to_string(Value) + "].Ambient";
			std::string LightDiffuse = "AllCnInfo[" + std::to_string(Value) + "].Diffuse";
			std::string LightSpecular = "AllCnInfo[" + std::to_string(Value) + "].Specular";
			ii->setVec3f(jj->GetAmbient(), LightAmbient.c_str());
			ii->setVec3f(jj->GetDiffuse(), LightDiffuse.c_str());
			ii->setVec3f(jj->GetSpecular(), LightSpecular.c_str());
			//Cone Information
			std::string LightCnAngle = "AllCnInfo[" + std::to_string(Value) + "].ConeAngle";
			ii->setVec1f(glm::cos(glm::radians(jj->GetCone())), LightCnAngle.c_str());
			Value++;
		}
		Value = 0;
		for (auto& jj : this->ArLights)
		{
			//General Light info
			std::string LightPos = "AllArInfo[" + std::to_string(Value) + "].Lightinf.LightPos";
			std::string LightClr = "AllArInfo[" + std::to_string(Value) + "].Lightinf.LightColor";
			std::string LightDir = "AllArInfo[" + std::to_string(Value) + "].Lightinf.LightDirection";

			ii->setVec3f(jj->GetPos(), LightPos.c_str());
			ii->setVec3f(jj->GetColor(), LightClr.c_str());
			ii->setVec3f(jj->GetFront(), LightDir.c_str());
			//Light Prop Info
			std::string LightAmbient = "AllArInfo[" + std::to_string(Value) + "].Lightinf.Ambient";
			std::string LightDiffuse = "AllArInfo[" + std::to_string(Value) + "].Lightinf.Diffuse";
			std::string LightSpecular = "AllArInfo[" + std::to_string(Value) + "].Lightinf.Specular";
			ii->setVec3f(jj->GetAmbient(), LightAmbient.c_str());
			ii->setVec3f(jj->GetDiffuse(), LightDiffuse.c_str());
			ii->setVec3f(jj->GetSpecular(), LightSpecular.c_str());
			//Cone Info
			std::string LightCnAngle = "AllArInfo[" + std::to_string(Value) + "].Lightinf.ConeAngle";
			std::string LightUmAngle = "AllArInfo[" + std::to_string(Value) + "].UmbraAngle";
			ii->setVec1f(glm::cos(glm::radians(jj->GetCone())), LightCnAngle.c_str());
			ii->setVec1f(glm::cos(glm::radians(jj->GetUmbra())),LightUmAngle.c_str());
			//Light Constants
			std::string LightLin = "AllArInfo["+ std::to_string(Value) + "].Linear";
			std::string LightConst = "AllArInfo[" + std::to_string(Value) + "].Constant";
			std::string LightQuad = "AllArInfo[" + std::to_string(Value) + "].Quadratic";
			ii->setVec1f(jj->GetConstant(), LightConst.c_str());
			ii->setVec1f(jj->GetLinear(), LightLin.c_str());
			ii->setVec1f(jj->GetQuad(), LightQuad.c_str());
		}
		ii->setMat4fv(this->ViewMatrix, "ViewMatrix");
		ii->setVec3f(this->camera.getPosition(), "cameraPos");
	}
	//Update FrameBuffer size and projection matrix
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

std::vector<glm::mat4> Game::updateShadows()
{
	std::vector<glm::mat4> ReturnMatrix;
		int Count = 0;
	for (auto& ii : this->Shadows)
		{
			glm::mat4 TempVal = this->LightsToUse[Count]->GetLightMatrix(this->worldUp);
			ii->WriteToBuffer(this->Window_Width, this->Window_Height,
				this->shaders[3],TempVal);
			for (auto& jj : this->models)
			{
				jj->RenderShadow(this->shaders[3]);
			}
			ReturnMatrix.push_back(TempVal);
			Count++;
		}
	return ReturnMatrix;
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
	this->initShadows();
	this->initTextures();
	this->initMaterials();
	this->initModels();
	this->initLights();
	this->initUniforms();
	this->camera.SetPos(glm::vec3(0.f, this->MipMapsData[HEIGHTMAP_1]->ReturnValue(0.f, 0.f) + 1.f,0.f));
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
	for (auto& ii : this->MatTest)
		delete ii;
	for (auto& i : meshes)
		delete i;
	for (size_t i = 0; i < this->shaders.size(); i++)
			delete this->shaders[i];
	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];
	for (auto& ii : this->LightsToUse)
		delete ii;
	for (auto*& i : this->models)
		delete i;
	for (auto& ii : this->MipMapsData)
		delete ii;	
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
	//Updating Shadows Textures
	std::vector<glm::mat4> TempVal = this->updateShadows();
	//Clear
	ImGui::Render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, this->Window_Width, this->Window_Height);
	glClearColor(this->SkyColor.r, this->SkyColor.g, this->SkyColor.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	//Update uniforms
	this->updateUniforms();
	//render Models
	for (auto& ii : this->models)
	{
		ii->TestRender(this->shaders,TempVal);
	} 
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