#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Vector.h"
#include "ECS.h"
#include "SDL_ttf.h"
#include "ColliderComponents.h"
#include "CollisionDetection.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"

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

	std::vector<Vector3D> boxCollider = {
	Vector3D(0.0f, 0.0f, 0),
	Vector3D(0.0f, 1.0f, 0),
	Vector3D(1.0f, 1.0f, 0),
	Vector3D(1.0f, 0.0f, 0)
	};

	newPlayer.addComponent<TransformComponent>(Vector2D(1, 1), Vector2D(1, 1));
	newPlayer.addComponent<SpriteRendererComponent>("Assets/player.png");
	newPlayer.getComponent<SpriteRendererComponent>().Initialize();
	newPlayer.addComponent<PlayerControllerComponent>();
	newPlayer.addComponent<ColliderComponent>(boxCollider, false);

	Box1.addComponent<TransformComponent>(Vector2D(2, 2), Vector2D(1, 1));
	Box1.addComponent<SpriteRendererComponent>("Assets/debug.png");
	Box1.getComponent<SpriteRendererComponent>().Initialize();
	Box1.addComponent<ColliderComponent>(boxCollider, false);

	Box2.addComponent<TransformComponent>(Vector2D(4, 2), Vector2D(1, 1));
	Box2.addComponent<SpriteRendererComponent>("Assets/debug.png");
	Box2.getComponent<SpriteRendererComponent>().Initialize();
	Box2.addComponent<ColliderComponent>(boxCollider, true);
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

auto collision = CollisionDetection();


void Game::update()
{
	manager.Update();	
	
	UpdateCollisions();
}

void Game::UpdateCollisions() 
{
	for (int i = 0; i < manager.entities.size(); i++)
	{
		Entity* entityA = manager.entities[i].get();
		if (entityA->hasComponent<ColliderComponent>())
		{
			for (int j = 0; j < manager.entities.size(); j++)
			{
				Entity* entityB = manager.entities[j].get();
				if (entityB->hasComponent<ColliderComponent>())
				{
					if (i == j)//Same Index Guard Clause
					{
						continue;
					}

					//If this gets laggy can do a sphere check before GJK algorithm

					auto collisionPoint = collision.GJK(&entityA->getComponent<ColliderComponent>(), &entityB->getComponent<ColliderComponent>());

					if (collisionPoint.colliding && !entityA->getComponent<ColliderComponent>().IsStatic())
					{
						entityA->getComponent<TransformComponent>().position.x -= collisionPoint.normal.x;
						entityA->getComponent<TransformComponent>().position.y -= collisionPoint.normal.y;
					}
				}
			}
		}
	}
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
