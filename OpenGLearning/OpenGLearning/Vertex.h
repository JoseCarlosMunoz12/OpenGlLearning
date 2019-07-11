#pragma once
#include <glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

struct MouseItems
{
	double offsetX;
	double offsetY;
};

struct MousePositions
{
	double MousePosX;
	double MousePosY;
};
struct FrameBufferItems
{
	int Width;
	int Height;
};