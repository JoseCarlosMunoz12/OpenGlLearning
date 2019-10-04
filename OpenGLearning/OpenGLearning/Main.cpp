#include "Game.h"

int main()
{
	glm::vec3 SkyColor = glm::vec3(0.f, 0.f, 1.f);
	Game game("Test Tile",1920,1080,4,5,false,SkyColor);

	// MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		game.update();
		game.render();
	}

	return 0;
}