#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Vector.h"
#include "ECS.h"
#include "Components.h"
#include "SDL_ttf.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;

auto& newPlayer(manager.AddEntity());
auto& debug(manager.AddEntity());

auto& Box1(manager.AddEntity());
auto& Box2(manager.AddEntity());


Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) 
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}


	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		std::cout << "Subsystems Initialised!..." << std::endl;

		window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (window) 
		{
			std::cout << "Window Created!..." << std::endl;
		}
		
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) 
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created!..." << std::endl;
		}

		if (TTF_Init() < 0) {
			std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
		}

		isRunning = true;
	}
	else 
	{
		isRunning = false;
	}

	map = new Map();
	newPlayer.addComponent<TransformComponent>(Vector2D(1, 1), Vector2D(1, 1));
	newPlayer.addComponent<SpriteRendererComponent>("Assets/player.png");
	newPlayer.getComponent<SpriteRendererComponent>().Initialize();
	newPlayer.addComponent<PlayerControllerComponent>();

	Box1.addComponent<TransformComponent>(Vector2D(2, 2), Vector2D(1, 1));
	//std::vector<Vector3D> vertexes = {
	//	Vector3D(1.0f, 1.0f, 0),
	//	Vector3D(1.0f, 2.0f, 0),
	//	Vector3D(2.0f, 2.0f, 0),
	//	Vector3D(2.0f, 1.0f, 0)
	//};
	//Box1.addComponent<Collider>(vertexes);

	Box2.addComponent<TransformComponent>(Vector2D(2, 2), Vector2D(1, 1));
	//std::vector<Vector3D> vertexes2 = {
	//Vector3D(2.5f, 1.5f, 0),
	//Vector3D(2.5f, 2.5f, 0),
	//Vector3D(3.5f, 2.5f, 0),
	//Vector3D(3.5f, 1.5f, 0)
	//};
	//Box2.addComponent<Collider>(vertexes2);
}

void Game::handleEvents()
{
	SDL_Event events;
	SDL_PollEvent(&events);
	switch (events.type) 
	{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			newPlayer.getComponent<PlayerControllerComponent>().KeyDown(&events.key);
			break;
		case SDL_KEYUP:
			newPlayer.getComponent<PlayerControllerComponent>().KeyUp(&events.key);
			break;
		default:
			break;
	}
}

void Game::update()
{
	manager.Update();

}

void Game::render()
{
	SDL_RenderClear(renderer);

	map->DrawMap();
	manager.Render();
	
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}
