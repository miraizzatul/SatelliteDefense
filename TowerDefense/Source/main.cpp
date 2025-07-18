#include "TowerDefense/Game.h"

int main(int argc, char* argv[])
{
	Game game;

	if (game.Init())//Start the game
	{
		game.Run();
	}

	game.Shutdown();
	return 0;
}