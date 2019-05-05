#pragma once

#include "libs.h"
#include "Camera.h"
#include <random>

//Enumerations
enum shader_enums{SHADER_CORE_PROGRAM,
					SHADER_TERRAIN,
					SHADERS_TERRAIN};
enum texture_enum {TEX_PUSHEEM,TEX_PUSHEEN_SPECULAR,
					TEX_CONTAINER,TEX_CONTAINER_SPECULAR,
					TEX_FLOWER,TEX_FLOWER_SPECULAR};
enum material_ennum {MAT_1 = 0,
					MAT_2,
					TERRAIN_MAT};
enum mesh_enum {MESH_QUAD = 0};

class Game
{
private:
	std::mt19937 rng;
	std::uniform_int_distribution<int> xDist;
	std::uniform_int_distribution<int> yDist;
	//Variables
	//Delta Time
	float dt;
	float cuTime;
	float lastTime;
	//Mouse Input
	double lastMouseX;
	double lastMouseY;
	double MouseX;
	double MouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;
	float Amount = 0;
	float AmountZ = 0;

	//window
	GLFWwindow* window;
	const int Window_Width;
	const int Window_Height;
	int frameBufferWidth;
	int frameBufferHeight;
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

	//Shaders
	std::vector<Shader*> shaders;
	//Textures
	std::vector<Texture*> textures;
	//Materials
	std::vector<Material*> materials;
	//Models
	std::vector<Model*> models;
	//Lights
	std::vector<glm::vec3*> lights;
	//Private Functions
	void initGLFW();
	void initWindow(const char* title,bool resizable);
	void initGLEW();
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initModels();
	void initLights();
	void initUniforms();

	void updateDT();
	void updateKeyboardInput();
	void updateMouseInput();
	void updateInput();

	void updateUniforms();
	void updateOpenGLOptions();
	//Static variables
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
};

