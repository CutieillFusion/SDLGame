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
#include "TextRendererComponent.h"
#include "AssetManager.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;

Vector2D Game::camera = Vector2D(0, 0);
Vector2D cameraOffset = Vector2D(800.0f/(WORLD_SCALE * 2), 640.0f/(WORLD_SCALE * 2));//Center Screen

Manager manager;

//Entities
auto& newPlayer(manager.AddEntity(LAYER_CHARACTER));
auto& debug(manager.AddEntity(LAYER_FOREGROUND));
auto& Box(manager.AddEntity(LAYER_FOREGROUND));
auto& BoxStatic(manager.AddEntity(LAYER_FOREGROUND));
auto& BoxTrigger(manager.AddEntity(LAYER_FOREGROUND));
auto& Text(manager.AddEntity(LAYER_UI));



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

		//Initializes AssetManager
		AssetManager();

		isRunning = true;
	}
	else 
	{
		isRunning = false;
	}

	//PUT ALL TEXTURES AND FONTS HERE
	AssetManager::instance->AddTexture("Player", "Assets/player.png");
	AssetManager::instance->AddTexture("Debug", "Assets/debug.png");
	AssetManager::instance->AddTexture("Water", "Assets/water.png");
	AssetManager::instance->AddTexture("Grass", "Assets/grass.png");
	AssetManager::instance->AddTexture("Dirt", "Assets/dirt.png");
	AssetManager::instance->AddFont("8Bit", "Assets/8Bit.ttf", 32);
	AssetManager::instance->AddFont("Test", "Assets/Test.ttf", 32);


	map = new Map(&manager);

	std::vector<Vector3D> boxCollider = {
	Vector3D(0.0f, 0.0f, 0),
	Vector3D(0.0f, 1.0f, 0),
	Vector3D(1.0f, 1.0f, 0),
	Vector3D(1.0f, 0.0f, 0)
	};
	
	newPlayer.addComponent<TransformComponent>(Vector3D(10, 10, 0), Vector3D(1, 1, 1));
	newPlayer.addComponent<SpriteRendererComponent>("Player");
	newPlayer.addComponent<PlayerControllerComponent>();
	newPlayer.addComponent<ColliderComponent>(boxCollider, false, false);
	newPlayer.addComponent<CameraComponent>();
	newPlayer.AddTag("Player");

	Box.addComponent<TransformComponent>(Vector3D(2, 2, 0), Vector3D(2, 1, 1));
	Box.addComponent<SpriteRendererComponent>("Debug");
	Box.addComponent<ColliderComponent>(boxCollider, false, false);
	Box.AddTag("Box");

	BoxStatic.addComponent<TransformComponent>(Vector3D(8, 4, 0), Vector3D(2, 3, 1));
	BoxStatic.addComponent<SpriteRendererComponent>("Debug");
	BoxStatic.addComponent<ColliderComponent>(boxCollider, true, false);
	BoxStatic.AddTag("Box");

	BoxTrigger.addComponent<TransformComponent>(Vector3D(6, 10, 0), Vector3D(2, 2, 1));
	BoxTrigger.addComponent<SpriteRendererComponent>("Debug");
	BoxTrigger.addComponent<ColliderComponent>(boxCollider, false, true);
	BoxTrigger.AddTag("Box");

	SDL_Color textColor = { 0, 0, 0, 255 };
	Text.addComponent<TextRendererComponent>("Test", "8Bit", Vector3D(10, 600, 0), textColor);

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
			newPlayer.getComponent<PlayerControllerComponent>().OnKeyDown(&events.key);
			break;
		case SDL_KEYUP:
			newPlayer.getComponent<PlayerControllerComponent>().OnKeyUp(&events.key);
			break;
		case SDL_MOUSEMOTION:
			int x, y;
			SDL_GetMouseState(&x, &y);
			//Text.getComponent<TextRendererComponent>().text = std::to_string(x) + " | " + std::to_string(y);
			break;
		default:
			break;
	}
}

auto collision = CollisionDetection();


void Game::update()
{
	manager.Update();
	
	camera.x = newPlayer.getComponent<TransformComponent>().position.x - cameraOffset.x;
	camera.y = newPlayer.getComponent<TransformComponent>().position.y - cameraOffset.y;

	UpdateCollisions();
}

void Game::UpdateCollisions() 
{
	for (int i = 0; i < manager.entities.size(); i++)
	{
		Entity* entityA = manager.entities[i].get();
		
		if (entityA->hasComponent<ColliderComponent>() && !entityA->getComponent<ColliderComponent>().IsTrigger())
		{
			ColliderComponent* colliderA = &entityA->getComponent<ColliderComponent>();
			for (int j = 0; j < manager.entities.size(); j++)
			{
				Entity* entityB = manager.entities[j].get();
				if (entityB->hasComponent<ColliderComponent>())
				{
					ColliderComponent* colliderB = &entityB->getComponent<ColliderComponent>();

					//Same Index Guard Clause
					if (i == j)
					{
						continue;
					}

					//Double Static Guard Clause
					if (colliderA->IsStatic() && colliderB->IsStatic()) 
					{
						continue;
					}

					//If this gets laggy can do a sphere check before GJK algorithm

					auto collisionPoint = collision.GJK(colliderA, colliderB);

					if (collisionPoint.colliding && !colliderA->IsStatic())
					{
						if (!colliderB->IsTrigger())
						{
							entityA->getComponent<TransformComponent>().dPosition.x -= collisionPoint.normal.x;
							entityA->getComponent<TransformComponent>().dPosition.y -= collisionPoint.normal.y;
						}
						else 
						{
							entityB->getComponent<ColliderComponent>().OnTrigger();
						}
					}
				}
			}
		}
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);

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
