#include "Game.h"

int main()
{
	Game game("Test Tile",640,480,4,5,false);

	// MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		game.update();
		game.render();
	}

	return 0;
}