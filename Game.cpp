#include "Game.h"
#include "Map.h"
#include "ColliderComponent.h"
#include "CollisionDetection.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "TextRendererComponent.h"
#include "ImageRendererComponent.h"
#include "AnimationComponent.h"
#include "AssetManager.h"
#include "JSONParser.h"
#include "UIColliderComponent.h"
#include "ButtonComponent.h"
#include "ButtonEvents.h"

Map* map;


SDL_Renderer* Game::renderer = nullptr;

Vector2D Game::camera = Vector2D(0, 0);
Vector2D cameraOffset = Vector2D(800.0f/(WORLD_SCALE * 2), 640.0f/(WORLD_SCALE * 2));//Center Screen

AssetManager* Game::assets = new AssetManager();

Manager* Game::manager = new Manager();

//Entities
auto& Player(Game::manager->AddEntity(LAYER_CHARACTER));
auto& debug(Game::manager->AddEntity(LAYER_FOREGROUND));
auto& Box(Game::manager->AddEntity(LAYER_FOREGROUND));
auto& BoxStatic(Game::manager->AddEntity(LAYER_FOREGROUND));
auto& BoxTrigger(Game::manager->AddEntity(LAYER_FOREGROUND));
auto& Text(Game::manager->AddEntity(LAYER_UI));
auto& Image(Game::manager->AddEntity(LAYER_UI));

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


	Game::assets->AddJSONFile("Sprites", "Assets/JSON/Sprites.json");
	Game::assets->LoadSpritesFromJSONFile("Sprites", "WorldSprites");
	Game::assets->LoadSpritesFromJSONFile("Sprites", "PlayerSprites");

	Game::assets->AddFont("8Bit", "Assets/Fonts/8Bit.ttf", 32);
	Game::assets->AddFont("Test", "Assets/Fonts/Test.ttf", 32);


	
	
	map = new Map(manager);

	std::vector<Vector3D> boxCollider = {
		Vector3D(0.0f, 0.0f, 0),
		Vector3D(0.0f, 1.0f, 0),
		Vector3D(1.0f, 1.0f, 0),
		Vector3D(1.0f, 0.0f, 0)
	};
	
	std::vector<std::string> player_sprites = 
	{
		"Player_0",
		"Player_1",
		"Player_2",
		"Player_3",
		"Player_4",
		"Player_5",
		"Player_6",
		"Player_7",
		"Player_8",
		"Player_9",
		"Player_10",
		"Player_11",
		"Player_12",
	};

	std::vector<AnimationState> player_animations =
	{
		{0, 144},
		{1, 144},
		{2, 144},
		{3, 144},
		{4, 144},
		{5, 144},
		{6, 144},
		{7, 144},
		{8, 144},
		{9, 144},
		{10, 144},
		{11, 144},
		{12, 144},
	};

	Player.addComponent<TransformComponent>(Vector3D(10, 10, 0), Vector3D(1, 1, 1));
	Player.addComponent<SpriteComponent>(player_sprites);
	Player.addComponent<AnimationComponent>(player_animations);
	Player.addComponent<SpriteRendererComponent>();
	Player.addComponent<PlayerControllerComponent>();
	Player.addComponent<ColliderComponent>(boxCollider, false, false);
	Player.AddTag("Player");

	Box.addComponent<TransformComponent>(Vector3D(2, 2, 0), Vector3D(2, 1, 1));
	Box.addComponent<SpriteComponent>("Debug");
	Box.addComponent<SpriteRendererComponent>();
	Box.addComponent<ColliderComponent>(boxCollider, false, false);
	Box.AddTag("Box");

	BoxStatic.addComponent<TransformComponent>(Vector3D(8, 4, 0), Vector3D(2, 3, 1));
	BoxStatic.addComponent<SpriteComponent>("Debug");
	BoxStatic.addComponent<SpriteRendererComponent>();
	BoxStatic.addComponent<ColliderComponent>(boxCollider, true, false);
	BoxStatic.AddTag("Box");

	BoxTrigger.addComponent<TransformComponent>(Vector3D(6, 10, 0), Vector3D(2, 2, 1));
	BoxTrigger.addComponent<SpriteComponent>("Debug");
	BoxTrigger.addComponent<SpriteRendererComponent>();
	BoxTrigger.addComponent<ColliderComponent>(boxCollider, false, true);
	BoxTrigger.AddTag("Box");

	SDL_Color textColor = { 0, 0, 0, 255 };
	Text.addComponent<RectComponent>(Vector3D(10, 600, 0), Vector3D(1, 1, 1));
	Text.addComponent<TextRendererComponent>("Test", "8Bit", textColor);

	std::vector<std::string> ids =
	{
		"Player_0", "Player_6", "Player_12"
	};
	std::vector<ButtonEvent> buttonEvents =
	{
		TestEvents::Test,
		TestEvents::Disable
	};
	Image.addComponent<RectComponent>(Vector3D(200, 400, 0), Vector3D(200, 200, 0));
	Image.addComponent<SpriteComponent>(ids);
	Image.addComponent<ImageRendererComponent>();
	Image.addComponent<UIColliderComponent>(boxCollider);
	Image.addComponent<ButtonComponent>(buttonEvents);
	Image.AddTag("Button");
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
		{
			auto inputListeners = manager->FindEntitiesWithTag("InputListenerComponent");
			for (auto inputListener : inputListeners)
			{
				Entity* entityA = manager->entities[inputListener].get();

				if (entityA->hasComponent<PlayerControllerComponent>())
				{
					entityA->getComponent<PlayerControllerComponent>().OnKeyDown(&events.key);
				}
				else if (entityA->hasComponent<ButtonComponent>())
				{
					entityA->getComponent<ButtonComponent>().OnKeyDown(&events.key);
				}
			}
			break;
		}
		case SDL_KEYUP:
		{
			auto inputListeners = manager->FindEntitiesWithTag("InputListenerComponent");
			for (auto inputListener : inputListeners)
			{
				Entity* entityA = manager->entities[inputListener].get();
				if (entityA->hasComponent<PlayerControllerComponent>())
				{
					entityA->getComponent<PlayerControllerComponent>().OnKeyUp(&events.key);
				}
				else if (entityA->hasComponent<ButtonComponent>())
				{
					entityA->getComponent<ButtonComponent>().OnKeyUp(&events.key);
				}
			}
			break;
		}
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mousePos.x, &mousePos.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			auto inputListeners = manager->FindEntitiesWithTag("InputListenerComponent");
			for (auto inputListener : inputListeners)
			{
				Entity* entityA = manager->entities[inputListener].get();

				if (entityA->hasComponent<PlayerControllerComponent>())
				{
					entityA->getComponent<PlayerControllerComponent>().OnMouseDown(&events.button);
				}
				else if (entityA->hasComponent<ButtonComponent>())
				{
					entityA->getComponent<ButtonComponent>().OnMouseDown(&events.button);
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			auto inputListeners = manager->FindEntitiesWithTag("InputListenerComponent");
			for (auto inputListener : inputListeners)
			{
				Entity* entityA = manager->entities[inputListener].get();

				if (entityA->hasComponent<PlayerControllerComponent>())
				{
					entityA->getComponent<PlayerControllerComponent>().OnMouseUp(&events.button);
				}
				else if (entityA->hasComponent<ButtonComponent>())
				{
					entityA->getComponent<ButtonComponent>().OnMouseUp(&events.button);
				}
			}
			break;
		}
		default:
			break;
	}
}

auto collision = CollisionDetection();

void Game::update()
{
	manager->Refresh();

	manager->Update();

	newCamera.x = Player.getComponent<TransformComponent>().position.x - cameraOffset.x;
	newCamera.y = Player.getComponent<TransformComponent>().position.y - cameraOffset.y;
	camera = Vector2D::Lerp(camera, newCamera, 3.5f * DELTA_TIME);

	UpdateCollisions();
	UpdateUICollisions();
}

void Game::UpdateCollisions() 
{
	for (std::map<UID, std::unique_ptr<Entity>>::iterator it = manager->entities.begin(); it != manager->entities.end(); ++it)
	{
		Entity* entityA = it->second.get();

		//Disabled Guard Clause
		if (!entityA->isActive()) 
		{
			continue;
		}

		if (entityA->hasComponent<ColliderComponent>() && !entityA->getComponent<ColliderComponent>().IsTrigger())
		{
			ColliderComponent* colliderA = &entityA->getComponent<ColliderComponent>();
			for (std::map<UID, std::unique_ptr<Entity>>::iterator _it = manager->entities.begin(); _it != manager->entities.end(); ++_it)
			{
				Entity* entityB = _it->second.get();
				
				//Disabled Guard Clause
				if (!entityB->isActive())
				{
					continue;
				}

				if (entityB->hasComponent<ColliderComponent>())
				{
					ColliderComponent* colliderB = &entityB->getComponent<ColliderComponent>();

					//Same Entity Guard Clause
					if (entityA->GetUID() == entityB->GetUID())
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

void Game::UpdateUICollisions()
{
	for (std::map<UID, std::unique_ptr<Entity>>::iterator it = manager->entities.begin(); it != manager->entities.end(); ++it)
	{
		Entity* entityA = it->second.get();

		//Disabled Guard Clause
		if (!entityA->isActive())
		{
			continue;
		}

		if (entityA->hasComponent<UIColliderComponent>())
		{
			UIColliderComponent* colliderA = &entityA->getComponent<UIColliderComponent>();

			colliderA->colliding = false;

			//If this gets laggy can do a sphere check before GJK algorithm
			if (collision.GJK(colliderA, &mousePos))
			{
				colliderA->colliding = true;
			}
		}
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);

	manager->Render();

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