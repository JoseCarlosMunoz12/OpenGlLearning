#pragma once

#include "libs.h"
#include "Camera.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Input.h"
#include <random>

//Enumerations
enum shader_enums
{
	SHADER_DEBUGS,
	SHADER_CORE_PROGRAM,
	SHADER_TERRAIN,
	SHADERS_TERRAIN,
	SHADER_SHADOWS
};
enum texture_enum 
{
	TEX_PUSHEEM,TEX_PUSHEEN_SPECULAR,
	TEX_CONTAINER,TEX_CONTAINER_SPECULAR,
	TEX_FLOWER,TEX_FLOWER_SPECULAR
};
enum material_ennum
{
	MAT_0,
	MAT_1 ,
	MAT_2
};
enum mesh_enum
{
	MESH_QUAD = 0,
	MESH_PLANE_TERRAIN,
	MESH_CUSTOM_TERRAIN,
	MESH_TRIANGLE,
	MESH_PYRAMID,
	MESH_CUBE,
	MESH_SPHERE,
	MESH_CYLINDER,
	MESH_CONE
};
enum heighMaps_enum
{
	HEIGHTMAP_1
};

class Game
{
private:
	//Variables
	//Delta Time
	float dt;
	float cuTime;
	float lastTime;
	//Mouse Input
	Mouse MouseToUse;

	float Amount = 0;
	float AmountZ = 0;
	
	int MapWidth;
	int MapHeigth;

	//window
	GLFWwindow* window;
	const int Window_Width;
	const int Window_Height;
	int frameBufferWidth;
	int frameBufferHeight;
	bool OtherWindow = true;
	//Camera
	Camera camera;
	//OpenGl context
	const int GLVerMajor;
	const int GLVerMinor;
	//Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;
	glm::vec3 SkyColor;
	bool MakeMesh = false;
	//Variable that changes item to click
	int ModelToMake = -1;
	int AnimModelToMake = -1;
	//Position where the Mouse Click does not affect
	ImVec2 ScreenPos;
	ImVec2 WinSize;
	//Colors of the ImGui name
	glm::vec3 ColorOfBackground = glm::vec3(1.f, 1.f, 1.f);
	//Shaders
	std::vector<Shader*> shaders;
	//Textures
	std::vector<GeneralTextInfo*> textures;
	std::vector<ShadowTex*> Shadows;
	std::vector<MipMap*> MipMapsData;
	//Materials
	//Testing the new mat with Polymorphism
	std::vector<StdMat*> MatTest;
	//Models
	std::vector<Model*> models;
	std::vector<AnimModel*> animModel;
	//Lights
	std::vector<MainLight*> LightsToUse;
	std::vector<DrLights*> DirLights;
	std::vector<PntLights*> PtLights;
	std::vector<ConeLights*> CnLights;
	std::vector<AreaLights*> ArLights;
	int DirLightsToShow = -1;
	int CnLightsToShow = -1;
	int ArLightsToShow = -1;
	// Meshes to reuse and not refind and make.
	std::vector<Mesh*> meshes;
	std::vector<AnimMesh*> animMeshes;
	//File reader and maker
	FileRdrMkr RdMkFiles;
	AnimFileRdrMkr AnimRdrMk;
	int FileID = -1;
	std::string FileSave = "";
	// Empty Variables for new models
	std::vector<Texture*> NewTex0;
	glm::vec3 SpaceLoc;
	Mesh* NewMesh;
	int CountMesh;
	//Values To Show Int
	int NewMatId = -1;
	int NewTexId0 = -1;
	int NewTexId1 = -1;
	int NewMeshID = -1;
	// Animation variables
	float TimePass;
	bool StarAnim = false;
	bool SliderAnim = false;
	bool EditAnim = false;
	bool AddAnim = false;
	bool BlendAnims = false;
	bool TransAnims = false;
	int CurBlend = 0;
	// Collision and PHysics world
	CPE::CollisionWorld* ColWorld;
	CPE::StaticCollisions* Statics;
	//Private Functions
	void initGLFW();
	void initWindow(const char* title,bool resizable);
	void initGLEW();
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initShadows();
	void initTextures();
	void initMaterials();
	void initModels();
	void initLights();
	void initUniforms();

	void updateDT();
	void updateKeyboardInput();
	void updateMouseInput();
	void updateInput();
	void updateController();
	
	std::vector<glm::mat4> updateShadows();

	void ImGuiOptions();

	void updateUniforms();
	void updateOpenGLOptions();
	//Static variables
	Input CheckCntrl;
public:
	//Constructors/destructors
	Game(const char * title,
		const int width, const int height,
		int GLmajorVer, int GLminorVer, bool resizable,glm::vec3 SkyColor);
	virtual ~Game();
	//Accessors
	int getWindowShouldClose();
	//Modifiers
	void setWindowShouldClose();
	//Functions
	void update();
	void render();
	//Static Functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
	//ImGui Functions	
	bool CheckNum(int Num);
	//Create New Models With Existing Meshes, Texture, etc.
	void LoadNewModels(std::vector<MdlToMake> FromFile);
	void ReleaseModels();
};

