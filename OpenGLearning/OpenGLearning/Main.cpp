#include "Game.h"

int main()
{
	//,640,480,
	Game game("Test Tile",1920,1080,4,5,false);

	// MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		game.update();
		game.render();
	}

	return 0;
}