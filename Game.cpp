#include "Game.h"
#include "Map.h"
#include "ECS.h"
#include "ColliderComponents.h"
#include "CollisionDetection.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "TextRendererComponent.h"
#include "ImageRendererComponent.h"
#include "AssetManager.h"

Map* map;


SDL_Renderer* Game::renderer = nullptr;

Vector2D Game::camera = Vector2D(0, 0);
Vector2D cameraOffset = Vector2D(800.0f/(WORLD_SCALE * 2), 640.0f/(WORLD_SCALE * 2));//Center Screen

Manager manager;

AssetManager* Game::assets = new AssetManager();

//Entities
auto& newPlayer(manager.AddEntity(LAYER_CHARACTER));
auto& debug(manager.AddEntity(LAYER_FOREGROUND));
auto& Box(manager.AddEntity(LAYER_FOREGROUND));
auto& BoxStatic(manager.AddEntity(LAYER_FOREGROUND));
auto& BoxTrigger(manager.AddEntity(LAYER_FOREGROUND));
auto& Text(manager.AddEntity(LAYER_UI));
auto& Image(manager.AddEntity(LAYER_UI));



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

	//PUT ALL TEXTURES AND FONTS HERE
	Game::assets->AddTexture("Player", "Assets/player.png");
	Game::assets->AddTexture("Debug", "Assets/debug.png");
	Game::assets->AddTexture("Water", "Assets/water.png");
	Game::assets->AddTexture("Grass", "Assets/grass.png");
	Game::assets->AddTexture("Dirt", "Assets/dirt.png");

	//Player Animations
	Game::assets->AddTexture("Player_0", "Assets/Player/Player_0.png");
	Game::assets->AddTexture("Player_1", "Assets/Player/Player_1.png");
	Game::assets->AddTexture("Player_2", "Assets/Player/Player_2.png");
	Game::assets->AddTexture("Player_3", "Assets/Player/Player_3.png");
	Game::assets->AddTexture("Player_4", "Assets/Player/Player_4.png");
	Game::assets->AddTexture("Player_5", "Assets/Player/Player_5.png");
	Game::assets->AddTexture("Player_6", "Assets/Player/Player_6.png");
	Game::assets->AddTexture("Player_7", "Assets/Player/Player_7.png");
	Game::assets->AddTexture("Player_8", "Assets/Player/Player_8.png");
	Game::assets->AddTexture("Player_9", "Assets/Player/Player_9.png");
	Game::assets->AddTexture("Player_10", "Assets/Player/Player_10.png");
	Game::assets->AddTexture("Player_11", "Assets/Player/Player_11.png");
	Game::assets->AddTexture("Player_12", "Assets/Player/Player_12.png");


	Game::assets->AddFont("8Bit", "Assets/Fonts/8Bit.ttf", 32);
	Game::assets->AddFont("Test", "Assets/Fonts/Test.ttf", 32);


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

	//SDL_Color textColor = { 0, 0, 0, 255 };
	//Text.addComponent<TextRendererComponent>("Test", "8Bit", Vector3D(10, 600, 0), textColor);

	//Image.addComponent<ImageRendererComponent>("Dirt", Vector3D(200, 400, 0), Vector3D(200, 200, 0));
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
