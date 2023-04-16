#include "Game.h"

Game* game = nullptr;

int main(int argc, char* argv[]) 
{
	const int FPS = 144;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init("Test", 100, 100, 1600, 900, false);

	while (game->running()) 
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) 
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}

//TODO:
//Add BarComponent
//Pokemon Battles
//	*Add Move Speed Check
//	*Add Death Check
//Add Circle Check before running CollisionDetection to save resources?
//Add Slerp to pivot point
//	*You hopefully remember the idea 
//	*while (dPosition > minMov) { position += minMov }
//https://www.youtube.com/watch?v=1eaxE_waDNc&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&index=8
//https://www.parallelrealities.co.uk/tutorials/