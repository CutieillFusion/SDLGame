#include "Game.h"

Game* game = nullptr;

int main(int argc, char* argv[]) 
{
	const int FPS = 144;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->Initialize("Test", 100, 100, 1600, 900, false);

	while (game->Running()) 
	{
		frameStart = SDL_GetTicks();

		game->HandleEvents();
		game->Update();
		game->Render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) 
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->Clean();

	return 0;
}

//TODO:
//Combine Texture Manager and Asset Manager
//Add Scenes
//	*Basically Global Parents for Entites thats easy to manage
//Add Local and Global Active for Game Object
//	*Pointer to Parent's Active Status?
//Add BarComponent
//Add Circle Check before running CollisionDetection to save resources?
//Fixed Weird Collision Bug 
//	*I think its a problem where they are colliding but the returned normal doesnt move it outside the object
//	*IDK How id fix this and honestly i wont unless it becomes a reoccuring issue



//https://www.youtube.com/watch?v=1eaxE_waDNc&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&index=8
//https://www.parallelrealities.co.uk/tutorials/

//Good C++ Memory Ideas to follow
//https://www.reddit.com/r/cpp_questions/comments/ul1996/getting_error_exception_thrown_read_access/