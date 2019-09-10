#include "Game.h"

int main()
{
	glm::vec3 SkyColor = glm::vec3(1.f, 1.f, 1.f);
	//,640,480,
	Game game("Test Tile",1920,1080,4,5,false,SkyColor);

	// MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		game.update();
		game.render();
	}

	return 0;
}