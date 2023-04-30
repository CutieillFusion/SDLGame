#include "Game.h"
#include "Map.h"
#include "AssetManager.h"
#include "CollisionDetection.h"
#include "..\Scripts\JSON Loader\JSONParser.h"
#include "..\Scripts\ECS\Components\ColliderComponent.h"
#include "..\Scripts\ECS\Components\SpriteRendererComponent.h"
#include "..\Scripts\ECS\Components\TransformComponent.h"
#include "..\Scripts\ECS\Components\PlayerControllerComponent.h"
#include "..\Scripts\ECS\Components\TextRendererComponent.h"
#include "..\Scripts\ECS\Components\ImageRendererComponent.h"
#include "..\Scripts\ECS\Components\AnimationComponent.h"
#include "..\Scripts\ECS\Components\UIColliderComponent.h"
#include "..\Scripts\ECS\Components\ButtonComponent.h"
#include "..\Scripts\ECS\Components\ButtonEvents.h"
#include "..\Scripts\ECS\Components\TrainerComponent.h"

#include "..\Scripts\Daemon\Daemon.h"
#include "..\Scripts\Daemon\Attack.h"
#include "..\Scripts\Daemon\Attacks.h"
#include "..\Scripts\Daemon\Battle.h"

//Map* map;

SDL_Renderer* Game::renderer = nullptr;

Vector2D Game::camera = Vector2D(0, 0);
Vector2D cameraOffset = Vector2D(1920.0f / (WORLD_SCALE * 2), 1080.0f / (WORLD_SCALE * 2));//Center Screen

AssetManager* Game::assets = new AssetManager();
Manager* Game::manager = new Manager();

CollisionDetection collision = CollisionDetection();

//Entities
#pragma region ENTITIES

#pragma region BattleUI
auto& Background(Game::manager->AddEntity(LAYER_UI));

auto& MoveButton0(Game::manager->AddEntity(LAYER_UI));
auto& MoveButton1(Game::manager->AddEntity(LAYER_UI));
auto& MoveButton2(Game::manager->AddEntity(LAYER_UI));
auto& MoveButton3(Game::manager->AddEntity(LAYER_UI));
auto& MoveButtonText0(Game::manager->AddEntity(LAYER_UI));
auto& MoveButtonText1(Game::manager->AddEntity(LAYER_UI));
auto& MoveButtonText2(Game::manager->AddEntity(LAYER_UI));
auto& MoveButtonText3(Game::manager->AddEntity(LAYER_UI));
auto& MoveButtonTextPP0(Game::manager->AddEntity(LAYER_UI));
auto& MoveButtonTextPP1(Game::manager->AddEntity(LAYER_UI));
auto& MoveButtonTextPP2(Game::manager->AddEntity(LAYER_UI));
auto& MoveButtonTextPP3(Game::manager->AddEntity(LAYER_UI));

auto& ItemSlot0(Game::manager->AddEntity(LAYER_UI));
auto& ItemSlot1(Game::manager->AddEntity(LAYER_UI));
auto& ItemSlot2(Game::manager->AddEntity(LAYER_UI));
auto& ItemSlot3(Game::manager->AddEntity(LAYER_UI));

auto& PlayerPokemonPlatform(Game::manager->AddEntity(LAYER_UI));
auto& PlayerPokemon(Game::manager->AddEntity(LAYER_UI));
auto& PlayerTeam0(Game::manager->AddEntity(LAYER_UI));
auto& PlayerTeam1(Game::manager->AddEntity(LAYER_UI));
auto& PlayerTeam2(Game::manager->AddEntity(LAYER_UI));
auto& PlayerTeam3(Game::manager->AddEntity(LAYER_UI));
auto& PlayerTeam4(Game::manager->AddEntity(LAYER_UI));
auto& PlayerTeam5(Game::manager->AddEntity(LAYER_UI));

auto& EnemyPokemonPlatform(Game::manager->AddEntity(LAYER_UI));
auto& EnemyPokemon(Game::manager->AddEntity(LAYER_UI));
auto& EnemyTeam0(Game::manager->AddEntity(LAYER_UI));
auto& EnemyTeam1(Game::manager->AddEntity(LAYER_UI));
auto& EnemyTeam2(Game::manager->AddEntity(LAYER_UI));
auto& EnemyTeam3(Game::manager->AddEntity(LAYER_UI));
auto& EnemyTeam4(Game::manager->AddEntity(LAYER_UI));
auto& EnemyTeam5(Game::manager->AddEntity(LAYER_UI));

auto& PlayerStatsBackground(Game::manager->AddEntity(LAYER_UI));
auto& PlayerStatsName(Game::manager->AddEntity(LAYER_UI));
auto& PlayerStatsLevel(Game::manager->AddEntity(LAYER_UI));
auto& PlayerStatsHeldItem(Game::manager->AddEntity(LAYER_UI));
auto& PlayerStatsHealth(Game::manager->AddEntity(LAYER_UI));
auto& PlayerStatsHealthText(Game::manager->AddEntity(LAYER_UI));

auto& EnemyStatsBackground(Game::manager->AddEntity(LAYER_UI));
auto& EnemyStatsName(Game::manager->AddEntity(LAYER_UI));
auto& EnemyStatsLevel(Game::manager->AddEntity(LAYER_UI));
auto& EnemyStatsHeldItem(Game::manager->AddEntity(LAYER_UI));
auto& EnemyStatsHealth(Game::manager->AddEntity(LAYER_UI));
auto& EnemyStatsHealthText(Game::manager->AddEntity(LAYER_UI));

auto& BattleTextBox(Game::manager->AddEntity(LAYER_UI));
auto& BattleUIParent(Game::manager->AddEntity(LAYER_UI));
#pragma endregion

#pragma region World
auto& WorldParent(Game::manager->AddEntity(LAYER_BACKGROUND));
auto& WorldBackground(Game::manager->AddEntity(LAYER_BACKGROUND));

auto& Player(Game::manager->AddEntity(LAYER_CHARACTER));
auto& Trainer(Game::manager->AddEntity(LAYER_CHARACTER));
auto& TrainerVision(Game::manager->AddEntity(LAYER_CHARACTER));
auto& Trainer2(Game::manager->AddEntity(LAYER_CHARACTER));
auto& Trainer2Vision(Game::manager->AddEntity(LAYER_CHARACTER));

auto& Box1(Game::manager->AddEntity(LAYER_FOREGROUND));
auto& Box2(Game::manager->AddEntity(LAYER_FOREGROUND));
auto& Box3(Game::manager->AddEntity(LAYER_FOREGROUND));
#pragma endregion

#pragma endregion

Game::Game()
{
} 

Game::~Game()
{
}

void Game::Initialize(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
	#pragma region Initialization

	screenSize = Vector2D((float)width, (float)height);
	cameraOffset = Vector2D(screenSize.x / (WORLD_SCALE * 2), screenSize.y / (WORLD_SCALE * 2));

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
	#pragma endregion

	#pragma region LoadJSONFiles
	assets->AddJSONFile("Sprites", "Assets/JSON/Sprites.json");
	assets->AddJSONFile("DaemonMoves", "Assets/JSON/DaemonMoves.json");
	assets->AddJSONFile("DaemonSpecies", "Assets/JSON/DaemonSpecies.json");
	assets->AddJSONFile("DaemonTeams", "Assets/JSON/DaemonTeams.json");

	assets->LoadSpritesFromJSONFile("Sprites", "WorldSprites");
	assets->LoadSpritesFromJSONFile("Sprites", "PlayerSprites");
	assets->LoadSpritesFromJSONFile("Sprites", "PokemonSprites");
	assets->LoadSpritesFromJSONFile("Sprites", "UISprites");

	assets->AddFont("8Bit36", "Assets/Fonts/8Bit.ttf", 36);
	assets->AddFont("8Bit56", "Assets/Fonts/8Bit.ttf", 56);
	assets->AddFont("Test", "Assets/Fonts/Test.ttf", 32);
	#pragma endregion

	std::vector<Vector3D> boxCollider =
	{
		Vector3D(0.0f, 0.0f, 0.0f),
		Vector3D(1.0f, 0.0f, 0.0f),
		Vector3D(1.0f, 1.0f, 0.0f),
		Vector3D(0.0f, 1.0f, 0.0f)
	};

	SDL_Color textColor = { 0, 0, 0, 255 };

	#pragma region BattleUI
	BattleUIParent.AddComponent<RectComponent>(Vector3D(0, 0, 0), Vector3D(0, 0, 0));

	Background.AddComponent<RectComponent>(Vector3D(0, 0, 0), Vector3D(screenSize.x, screenSize.y, 0));
	Background.AddComponent<SpriteComponent>("Grass");
	Background.AddComponent<ImageRendererComponent>();
	Background.SetParent(&BattleUIParent);

	#pragma region ITEM_SLOTS
	std::vector<std::string> itemSlotIds =
	{
		"ItemSlot", "ItemSlot_Highlighted", "ItemSlot_Activated"
	};

	Vector3D itemSlotScale = Vector3D(screenSize.x / 12.0f, screenSize.y * 4.0f / 27.0f, 0);
	ItemSlot0.AddComponent<RectComponent>(Vector3D(screenSize.x * 8.0f / 12.0f, screenSize.y * 23.0f / 27.0f, 0), itemSlotScale);
	ItemSlot0.AddComponent<SpriteComponent>(itemSlotIds);
	ItemSlot0.AddComponent<ImageRendererComponent>();
	ItemSlot0.AddComponent<UIColliderComponent>(boxCollider);
	ItemSlot0.AddComponent<ButtonComponent>();
	ItemSlot0.SetParent(&BattleUIParent);

	ItemSlot1.AddComponent<RectComponent>(Vector3D(screenSize.x * 1.0f / 12.0f, 0, 0), itemSlotScale);
	ItemSlot1.AddComponent<SpriteComponent>(itemSlotIds);
	ItemSlot1.AddComponent<ImageRendererComponent>();
	ItemSlot1.AddComponent<UIColliderComponent>(boxCollider);
	ItemSlot1.AddComponent<ButtonComponent>();
	ItemSlot1.SetParent(&ItemSlot0);

	ItemSlot2.AddComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 12.0f, 0, 0), itemSlotScale);
	ItemSlot2.AddComponent<SpriteComponent>(itemSlotIds);
	ItemSlot2.AddComponent<ImageRendererComponent>();
	ItemSlot2.AddComponent<UIColliderComponent>(boxCollider);
	ItemSlot2.AddComponent<ButtonComponent>();
	ItemSlot2.SetParent(&ItemSlot0);

	ItemSlot3.AddComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 12.0f, 0, 0), itemSlotScale);
	ItemSlot3.AddComponent<SpriteComponent>(itemSlotIds);
	ItemSlot3.AddComponent<ImageRendererComponent>();
	ItemSlot3.AddComponent<UIColliderComponent>(boxCollider);
	ItemSlot3.AddComponent<ButtonComponent>();
	ItemSlot3.SetParent(&ItemSlot0);
	#pragma endregion

	#pragma region POKEMON_PLATFORMS
	Vector3D pokemonPlatformScale = Vector3D(384, screenSize.y * 16.0f / 45.0f, 0);
	Vector3D pokemonScale = Vector3D(screenSize.x * 9.0f / 50.0f, screenSize.y * 8.0f / 25.0f, 0);

	PlayerPokemonPlatform.AddComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 80.0f, screenSize.y * 2.0f / 3.0f, 0), pokemonPlatformScale);
	PlayerPokemonPlatform.AddComponent<SpriteComponent>("Pokemon_Platform");
	PlayerPokemonPlatform.AddComponent<ImageRendererComponent>();
	PlayerPokemonPlatform.SetParent(&BattleUIParent);

	PlayerPokemon.AddComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 50.0f, screenSize.y * 128.0f / 225.0f, 0), pokemonScale);
	PlayerPokemon.AddComponent<SpriteComponent>("Clawrean");
	PlayerPokemon.AddComponent<ImageRendererComponent>();
	PlayerPokemon.SetParent(&BattleUIParent);

	EnemyPokemonPlatform.AddComponent<RectComponent>(Vector3D(screenSize.x * 43.0f / 100.0f, screenSize.y / 9.0f, 0), pokemonPlatformScale);
	EnemyPokemonPlatform.AddComponent<SpriteComponent>("Pokemon_Platform");
	EnemyPokemonPlatform.AddComponent<ImageRendererComponent>();
	EnemyPokemonPlatform.SetParent(&BattleUIParent);

	EnemyPokemon.AddComponent<RectComponent>(Vector3D(screenSize.x * 47.0f / 100.0f, screenSize.y * 8.0f / 225.0f, 0), pokemonScale);
	EnemyPokemon.AddComponent<SpriteComponent>("Crungly");
	EnemyPokemon.AddComponent<ImageRendererComponent>();
	EnemyPokemon.SetParent(&BattleUIParent);
	#pragma endregion

	#pragma region PLAYER_STATS
	PlayerStatsBackground.AddComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 10.0f, screenSize.y * 32.0f / 45.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, screenSize.y * 32.0f / 225.0f, 0));
	PlayerStatsBackground.AddComponent<SpriteComponent>("Stats_Border");
	PlayerStatsBackground.AddComponent<ImageRendererComponent>();
	PlayerStatsBackground.SetParent(&BattleUIParent);

	PlayerStatsName.AddComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 400.0f, screenSize.y * 1.0f / 225.0f, 0), Vector3D(screenSize.x * 2.0f / 25.0f, screenSize.y * 8.0f / 225.0f, 0));
	PlayerStatsName.AddComponent<TextRendererComponent>("Name", "8Bit36", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	PlayerStatsName.SetParent(&PlayerStatsBackground);

	PlayerStatsLevel.AddComponent<RectComponent>(Vector3D(-12, screenSize.y * 1.0f / 225.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, 32, 0));
	PlayerStatsLevel.AddComponent<TextRendererComponent>("Lvl: 01", "8Bit36", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	PlayerStatsLevel.SetParent(&PlayerStatsBackground);

	PlayerStatsHealth.AddComponent<RectComponent>(Vector3D(screenSize.x * 1.0f / 100.0f, screenSize.y * 4.0f / 75.0f, 0), Vector3D(504, 28, 0));
	PlayerStatsHealth.AddComponent<SpriteComponent>("Health_Bar");
	PlayerStatsHealth.AddComponent<ImageRendererComponent>();
	PlayerStatsHealth.SetParent(&PlayerStatsBackground);

	PlayerStatsHealthText.AddComponent<RectComponent>(Vector3D(-24, screenSize.y * 22.0f / 225.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, 32, 0));
	PlayerStatsHealthText.AddComponent<TextRendererComponent>("300/350", "8Bit36", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	PlayerStatsHealthText.SetParent(&PlayerStatsBackground);
	#pragma endregion

	#pragma region ENEMY_STATS
	EnemyStatsBackground.AddComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 200.0f, screenSize.y * 2.0f / 75.0f, 0), Vector3D(screenSize.x * 17.0f / 50.f, screenSize.y * 32.0f / 225.0f, 0));
	EnemyStatsBackground.AddComponent<SpriteComponent>("Stats_Border");
	EnemyStatsBackground.AddComponent<ImageRendererComponent>();
	EnemyStatsBackground.SetParent(&BattleUIParent);

	EnemyStatsName.AddComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 400.0f, screenSize.y * 1.0f / 225.0f, 0), Vector3D(screenSize.x * 2.0f / 25.0f, screenSize.y * 8.0f / 225.0f, 0));
	EnemyStatsName.AddComponent<TextRendererComponent>("Name", "8Bit36", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	EnemyStatsName.SetParent(&EnemyStatsBackground);

	EnemyStatsLevel.AddComponent<RectComponent>(Vector3D(-12, screenSize.y * 1.0f / 225.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, screenSize.y * 8.0f / 225.0f, 0));
	EnemyStatsLevel.AddComponent<TextRendererComponent>("Lvl: 01", "8Bit36", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	EnemyStatsLevel.SetParent(&EnemyStatsBackground);

	EnemyStatsHealth.AddComponent<RectComponent>(Vector3D(screenSize.x * 1.0f / 100.0f, screenSize.y * 4.0f / 75.0f, 0), Vector3D(screenSize.x * 63.0f / 200.0f, screenSize.y * 7.0f / 225.0f, 0));
	EnemyStatsHealth.AddComponent<SpriteComponent>("Health_Bar");
	EnemyStatsHealth.AddComponent<ImageRendererComponent>();
	EnemyStatsHealth.SetParent(&EnemyStatsBackground);

	EnemyStatsHealthText.AddComponent<RectComponent>(Vector3D(-24, screenSize.y * 22.0f / 225.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, screenSize.y * 8.0f / 225.0f, 0));
	EnemyStatsHealthText.AddComponent<TextRendererComponent>("300/350", "8Bit36", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	EnemyStatsHealthText.SetParent(&EnemyStatsBackground);
	#pragma endregion 

	#pragma region POKEMON_TEAM
	std::vector<std::string> pokeballIds =
	{
		"Pokeball_Team", "Pokeball_Team_Highlighted", "Pokeball_Team_Activated"
	};

	Vector3D pokeballScale = Vector3D(80, 80, 0);
	auto swapFunc = Daemon::Model::BattleWrapper::SwapDaemon;

	PlayerTeam0.AddComponent<RectComponent>(Vector3D(0, 144, 0), pokeballScale);
	PlayerTeam0.AddComponent<SpriteComponent>(pokeballIds);
	PlayerTeam0.AddComponent<ImageRendererComponent>();
	PlayerTeam0.AddComponent<UIColliderComponent>(boxCollider);
	PlayerTeam0.AddComponent<ButtonComponent>(swapFunc);
	PlayerTeam0.GetComponent<ButtonComponent>().data = { 0 };
	PlayerTeam0.SetParent(&PlayerStatsBackground);

	PlayerTeam1.AddComponent<RectComponent>(Vector3D(92, 144, 0), pokeballScale);
	PlayerTeam1.AddComponent<SpriteComponent>(pokeballIds);
	PlayerTeam1.AddComponent<ImageRendererComponent>();
	PlayerTeam1.AddComponent<UIColliderComponent>(boxCollider);
	PlayerTeam1.AddComponent<ButtonComponent>(swapFunc);
	PlayerTeam1.GetComponent<ButtonComponent>().data = { 1 };
	PlayerTeam1.SetParent(&PlayerStatsBackground);

	PlayerTeam2.AddComponent<RectComponent>(Vector3D(184, 144, 0), pokeballScale);
	PlayerTeam2.AddComponent<SpriteComponent>(pokeballIds);
	PlayerTeam2.AddComponent<ImageRendererComponent>();
	PlayerTeam2.AddComponent<UIColliderComponent>(boxCollider);
	PlayerTeam2.AddComponent<ButtonComponent>(swapFunc);
	PlayerTeam2.GetComponent<ButtonComponent>().data = { 2 };
	PlayerTeam2.SetParent(&PlayerStatsBackground);

	PlayerTeam3.AddComponent<RectComponent>(Vector3D(276, 144, 0), pokeballScale);
	PlayerTeam3.AddComponent<SpriteComponent>(pokeballIds);
	PlayerTeam3.AddComponent<ImageRendererComponent>();
	PlayerTeam3.AddComponent<UIColliderComponent>(boxCollider);
	PlayerTeam3.AddComponent<ButtonComponent>(swapFunc);
	PlayerTeam3.GetComponent<ButtonComponent>().data = { 3 };
	PlayerTeam3.SetParent(&PlayerStatsBackground);

	PlayerTeam4.AddComponent<RectComponent>(Vector3D(368, 144, 0), pokeballScale);
	PlayerTeam4.AddComponent<SpriteComponent>(pokeballIds);
	PlayerTeam4.AddComponent<ImageRendererComponent>();
	PlayerTeam4.AddComponent<UIColliderComponent>(boxCollider);
	PlayerTeam4.AddComponent<ButtonComponent>(swapFunc);
	PlayerTeam4.GetComponent<ButtonComponent>().data = { 4 };
	PlayerTeam4.SetParent(&PlayerStatsBackground);

	PlayerTeam5.AddComponent<RectComponent>(Vector3D(460, 144, 0), pokeballScale);
	PlayerTeam5.AddComponent<SpriteComponent>(pokeballIds);
	PlayerTeam5.AddComponent<ImageRendererComponent>();
	PlayerTeam5.AddComponent<UIColliderComponent>(boxCollider);
	PlayerTeam5.AddComponent<ButtonComponent>(swapFunc);
	PlayerTeam5.GetComponent<ButtonComponent>().data = { 5 };
	PlayerTeam5.SetParent(&PlayerStatsBackground);

	EnemyTeam0.AddComponent<RectComponent>(Vector3D(0, 144, 0), pokeballScale);
	EnemyTeam0.AddComponent<SpriteComponent>(pokeballIds);
	EnemyTeam0.AddComponent<ImageRendererComponent>();
	EnemyTeam0.AddComponent<UIColliderComponent>(boxCollider);
	EnemyTeam0.AddComponent<ButtonComponent>();
	EnemyTeam0.SetParent(&EnemyStatsBackground);

	EnemyTeam1.AddComponent<RectComponent>(Vector3D(92, 144, 0), pokeballScale);
	EnemyTeam1.AddComponent<SpriteComponent>(pokeballIds);
	EnemyTeam1.AddComponent<ImageRendererComponent>();
	EnemyTeam1.AddComponent<UIColliderComponent>(boxCollider);
	EnemyTeam1.AddComponent<ButtonComponent>();
	EnemyTeam1.SetParent(&EnemyStatsBackground);

	EnemyTeam2.AddComponent<RectComponent>(Vector3D(184, 144, 0), pokeballScale);
	EnemyTeam2.AddComponent<SpriteComponent>(pokeballIds);
	EnemyTeam2.AddComponent<ImageRendererComponent>();
	EnemyTeam2.AddComponent<UIColliderComponent>(boxCollider);
	EnemyTeam2.AddComponent<ButtonComponent>();
	EnemyTeam2.SetParent(&EnemyStatsBackground);

	EnemyTeam3.AddComponent<RectComponent>(Vector3D(276, 144, 0), pokeballScale);
	EnemyTeam3.AddComponent<SpriteComponent>(pokeballIds);
	EnemyTeam3.AddComponent<ImageRendererComponent>();
	EnemyTeam3.AddComponent<UIColliderComponent>(boxCollider);
	EnemyTeam3.AddComponent<ButtonComponent>();
	EnemyTeam3.SetParent(&EnemyStatsBackground);

	EnemyTeam4.AddComponent<RectComponent>(Vector3D(368, 144, 0), pokeballScale);
	EnemyTeam4.AddComponent<SpriteComponent>(pokeballIds);
	EnemyTeam4.AddComponent<ImageRendererComponent>();
	EnemyTeam4.AddComponent<UIColliderComponent>(boxCollider);
	EnemyTeam4.AddComponent<ButtonComponent>();
	EnemyTeam4.SetParent(&EnemyStatsBackground);

	EnemyTeam5.AddComponent<RectComponent>(Vector3D(460, 144, 0), pokeballScale);
	EnemyTeam5.AddComponent<SpriteComponent>(pokeballIds);
	EnemyTeam5.AddComponent<ImageRendererComponent>();
	EnemyTeam5.AddComponent<UIColliderComponent>(boxCollider);
	EnemyTeam5.AddComponent<ButtonComponent>();
	EnemyTeam5.SetParent(&EnemyStatsBackground);
	#pragma endregion

	#pragma region MOVE_BUTTONS
	std::vector<std::string> moveButton =
	{
		"Move_Normal", "Move_Normal_Highlighted", "Move_Normal_Activated"
	};
	Vector3D moveButtonSize = Vector3D(screenSize.x / 3.0f, screenSize.y * 1.0f / 15.0f + 1, 0);
	auto moveFunc = Daemon::Model::BattleWrapper::UseMove;

	MoveButton0.AddComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 500, 0), moveButtonSize);
	MoveButton0.AddComponent<SpriteComponent>(moveButton);
	MoveButton0.AddComponent<ImageRendererComponent>();
	MoveButton0.AddComponent<UIColliderComponent>(boxCollider);
	MoveButton0.AddComponent<ButtonComponent>(moveFunc);
	MoveButton0.GetComponent<ButtonComponent>().data = { 0 };
	MoveButton0.SetParent(&BattleUIParent);

	MoveButtonText0.AddComponent<RectComponent>(Vector3D(12, 4, 0), moveButtonSize);
	MoveButtonText0.AddComponent<TextRendererComponent>("Tackle", "8Bit56", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	MoveButtonText0.SetParent(&MoveButton0);

	MoveButtonTextPP0.AddComponent<RectComponent>(Vector3D(0, 4, 0), moveButtonSize);
	MoveButtonTextPP0.AddComponent<TextRendererComponent>("35/35", "8Bit56", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	MoveButtonTextPP0.SetParent(&MoveButton0);

	MoveButton1.AddComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 560, 0), moveButtonSize);
	MoveButton1.AddComponent<SpriteComponent>(moveButton);
	MoveButton1.AddComponent<ImageRendererComponent>();
	MoveButton1.AddComponent<UIColliderComponent>(boxCollider);
	MoveButton1.AddComponent<ButtonComponent>(moveFunc);
	MoveButton1.GetComponent<ButtonComponent>().data = { 1 };
	MoveButton1.SetParent(&BattleUIParent);

	MoveButtonText1.AddComponent<RectComponent>(Vector3D(12, 4, 0), moveButtonSize);
	MoveButtonText1.AddComponent<TextRendererComponent>("Leer", "8Bit56", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	MoveButtonText1.SetParent(&MoveButton1);

	MoveButtonTextPP1.AddComponent<RectComponent>(Vector3D(0, 4, 0), moveButtonSize);
	MoveButtonTextPP1.AddComponent<TextRendererComponent>("30/30", "8Bit56", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	MoveButtonTextPP1.SetParent(&MoveButton1);

	MoveButton2.AddComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 620, 0), moveButtonSize);
	MoveButton2.AddComponent<SpriteComponent>(moveButton);
	MoveButton2.AddComponent<ImageRendererComponent>();
	MoveButton2.AddComponent<UIColliderComponent>(boxCollider);
	MoveButton2.AddComponent<ButtonComponent>(moveFunc);
	MoveButton2.GetComponent<ButtonComponent>().data = { 2 };
	MoveButton2.SetParent(&BattleUIParent);

	MoveButtonText2.AddComponent<RectComponent>(Vector3D(12, 4, 0), moveButtonSize);
	MoveButtonText2.AddComponent<TextRendererComponent>("False Swipe", "8Bit56", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	MoveButtonText2.SetParent(&MoveButton2);

	MoveButtonTextPP2.AddComponent<RectComponent>(Vector3D(0, 4, 0), moveButtonSize);
	MoveButtonTextPP2.AddComponent<TextRendererComponent>("40/40", "8Bit56", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	MoveButtonTextPP2.SetParent(&MoveButton2);

	MoveButton3.AddComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 680, 0), moveButtonSize);
	MoveButton3.AddComponent<SpriteComponent>(moveButton);
	MoveButton3.AddComponent<ImageRendererComponent>();
	MoveButton3.AddComponent<UIColliderComponent>(boxCollider);
	MoveButton3.AddComponent<ButtonComponent>(moveFunc);
	MoveButton3.GetComponent<ButtonComponent>().data = { 3 };
	MoveButton3.SetParent(&BattleUIParent);

	MoveButtonText3.AddComponent<RectComponent>(Vector3D(12, 4, 0), moveButtonSize);
	MoveButtonText3.AddComponent<TextRendererComponent>("Protect", "8Bit56", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	MoveButtonText3.SetParent(&MoveButton3);

	MoveButtonTextPP3.AddComponent<RectComponent>(Vector3D(0, 4, 0), moveButtonSize);
	MoveButtonTextPP3.AddComponent<TextRendererComponent>("10/10", "8Bit56", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	MoveButtonTextPP3.SetParent(&MoveButton3);
	#pragma endregion

	BattleTextBox.AddComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 400, 0), Vector3D(screenSize.x / 3.0f, 80, 0));
	BattleTextBox.AddComponent<SpriteComponent>("BattleTextBox");
	BattleTextBox.AddComponent<ImageRendererComponent>(); 
	BattleTextBox.AddComponent<TextRendererComponent>(" SELECT NEXT MOVE...", "8Bit56", textColor, TEXT_ALIGN_CENTER, TEXT_ALIGN_MIDDLE);
	BattleTextBox.SetParent(&BattleUIParent);
	#pragma endregion

	Daemon::Model::DaeTeam* playerTeam = assets->CreateDaeTeam("Player");
	Daemon::Model::DaeTeam* trainerTeam = assets->CreateDaeTeam("Trainer");
	Daemon::Model::DaeTeam* trainer2Team = assets->CreateDaeTeam("Trainer2");

	#pragma region World

	WorldParent.AddComponent<TransformComponent>(Vector3D(0, 0, 0), Vector3D(0, 0, 0));
	 
	std::vector<std::string> playerSpriteIds =
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
		"Player_12"
	};

	std::vector<AnimationState> playerAnimation =
	{
		{ 0, 15 },
		{ 1, 15 },
		{ 2, 15 },
		{ 3, 15 },
		{ 4, 15 },
		{ 5, 15 },
		{ 6, 15 },
		{ 7, 15 },
		{ 8, 15 },
		{ 9, 15 },
		{ 10, 15 },
		{ 11, 15 },
		{ 12, 15 }
	};

	Player.AddComponent<TransformComponent>(Vector3D(0, 0, 0), Vector3D(1, 1, 0));
	Player.AddComponent<SpriteComponent>(playerSpriteIds);
	Player.AddComponent<SpriteRendererComponent>();
	Player.AddComponent<AnimationComponent>(playerAnimation);
	Player.AddComponent<PlayerControllerComponent>();
	Player.AddComponent<ColliderComponent>(boxCollider, false, false);
	Player.AddComponent<TrainerComponent>(playerTeam);
	Player.AddTag("Player");
	Player.SetParent(&WorldParent);

	Trainer.AddComponent<TransformComponent>(Vector3D(3, -2, 0), Vector3D(1, 1, 0));
	Trainer.AddComponent<SpriteComponent>("Dirt");
	Trainer.AddComponent<SpriteRendererComponent>();
	Trainer.AddComponent<ColliderComponent>(boxCollider, true, false);
	Trainer.AddComponent<TrainerComponent>(trainerTeam);
	Trainer.SetParent(&WorldParent);

	TrainerVision.AddComponent<TransformComponent>(Vector3D(-3, 0, 0), Vector3D(3, 1, 0));
	TrainerVision.AddComponent<SpriteComponent>("Debug");//TEMP
	TrainerVision.AddComponent<SpriteRendererComponent>();//TEMP
	TrainerVision.AddComponent<ColliderComponent>(boxCollider, false, true);
	TrainerVision.SetParent(&Trainer);

	Trainer2.AddComponent<TransformComponent>(Vector3D(-3, -3, 0), Vector3D(1, 1, 0));
	Trainer2.AddComponent<SpriteComponent>("Dirt");
	Trainer2.AddComponent<SpriteRendererComponent>();
	Trainer2.AddComponent<ColliderComponent>(boxCollider, true, false);
	Trainer2.AddComponent<TrainerComponent>(trainer2Team);
	Trainer2.SetParent(&WorldParent);

	Trainer2Vision.AddComponent<TransformComponent>(Vector3D(0, -3, 0), Vector3D(1, 3, 0));
	Trainer2Vision.AddComponent<SpriteComponent>("Debug");//TEMP
	Trainer2Vision.AddComponent<SpriteRendererComponent>();//TEMP
	Trainer2Vision.AddComponent<ColliderComponent>(boxCollider, false, true);
	Trainer2Vision.SetParent(&Trainer2);

	Box1.AddComponent<TransformComponent>(Vector3D(1, 1, 0), Vector3D(1, 1, 0));
	Box1.AddComponent<SpriteComponent>("Grass");
	Box1.AddComponent<SpriteRendererComponent>();
	Box1.AddComponent<ColliderComponent>(boxCollider, true, false);
	Box1.SetParent(&WorldParent);

	Box2.AddComponent<TransformComponent>(Vector3D(3, 7, 0), Vector3D(1, 1, 0));
	Box2.AddComponent<SpriteComponent>("Grass");
	Box2.AddComponent<SpriteRendererComponent>();
	Box2.AddComponent<ColliderComponent>(boxCollider, true, false);
	Box2.SetParent(&WorldParent);

	Box3.AddComponent<TransformComponent>(Vector3D(2, 5, 0), Vector3D(1, 1, 0));
	Box3.AddComponent<SpriteComponent>("Grass");
	Box3.AddComponent<SpriteRendererComponent>();
	Box3.AddComponent<ColliderComponent>(boxCollider, false, false);
	Box3.SetParent(&WorldParent);

	#pragma endregion 

	BattleUIParent.SetActiveStatus(false);

	std::map<std::string, Entity*> ui =
	{
		{"PlayerPokemonPlatform", &PlayerPokemonPlatform},
		{"PlayerPokemon", &PlayerPokemon},
		{"EnemyPokemonPlatform", &EnemyPokemonPlatform},
		{"EnemyPokemon", &EnemyPokemon},
		{"PlayerStatsName", &PlayerStatsName},
		{"PlayerStatsLevel", &PlayerStatsLevel},
		{"PlayerStatsHealth", &PlayerStatsHealth},
		{"PlayerStatsHealthText", &PlayerStatsHealthText},
		{"EnemyStatsName", &EnemyStatsName},
		{"EnemyStatsLevel", &EnemyStatsLevel},
		{"EnemyStatsHealth", &EnemyStatsHealth},
		{"EnemyStatsHealthText", &EnemyStatsHealthText},
		{"PlayerTeam0", &PlayerTeam0},
		{"PlayerTeam1", &PlayerTeam1},
		{"PlayerTeam2", &PlayerTeam2},
		{"PlayerTeam3", &PlayerTeam3},
		{"PlayerTeam4", &PlayerTeam4},
		{"PlayerTeam5", &PlayerTeam5},
		{"EnemyTeam0", &EnemyTeam0},
		{"EnemyTeam1", &EnemyTeam1},
		{"EnemyTeam2", &EnemyTeam2},
		{"EnemyTeam3", &EnemyTeam3},
		{"EnemyTeam4", &EnemyTeam4},
		{"EnemyTeam5", &EnemyTeam5},
		{"MoveButton0", &MoveButton0},
		{"MoveButtonText0", &MoveButtonText0},
		{"MoveButtonTextPP0", &MoveButtonTextPP0},
		{"MoveButton1", &MoveButton1},
		{"MoveButtonText1" ,&MoveButtonText1},
		{"MoveButtonTextPP1", &MoveButtonTextPP1},
		{"MoveButton2", &MoveButton2},
		{"MoveButtonText2", &MoveButtonText2},
		{"MoveButtonTextPP2", &MoveButtonTextPP2},
		{"MoveButton3", &MoveButton3},
		{"MoveButtonText3", &MoveButtonText3},
		{"MoveButtonTextPP3", &MoveButtonTextPP3},
		{"BattleTextBox", &BattleTextBox},
		{"BattleUIParent", &BattleUIParent},
		{"WorldParent", &WorldParent}
	};

	Daemon::Model::Battle::InitalizeUI(ui);
}

void Game::HandleEvents()
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

				if (entityA->HasComponent<PlayerControllerComponent>())
				{
					entityA->GetComponent<PlayerControllerComponent>().OnKeyDown(&events.key);
				}
				else if (entityA->HasComponent<ButtonComponent>())
				{
					entityA->GetComponent<ButtonComponent>().OnKeyDown(&events.key);
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
				if (entityA->HasComponent<PlayerControllerComponent>())
				{
					entityA->GetComponent<PlayerControllerComponent>().OnKeyUp(&events.key);
				}
				else if (entityA->HasComponent<ButtonComponent>())
				{
					entityA->GetComponent<ButtonComponent>().OnKeyUp(&events.key);
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

				if (entityA->HasComponent<PlayerControllerComponent>())
				{
					entityA->GetComponent<PlayerControllerComponent>().OnMouseDown(&events.button);
				}
				else if (entityA->HasComponent<ButtonComponent>())
				{
					entityA->GetComponent<ButtonComponent>().OnMouseDown(&events.button);
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

				if (entityA->HasComponent<PlayerControllerComponent>())
				{
					entityA->GetComponent<PlayerControllerComponent>().OnMouseUp(&events.button);
				}
				else if (entityA->HasComponent<ButtonComponent>())
				{
					entityA->GetComponent<ButtonComponent>().OnMouseUp(&events.button);
				}
			}
			break;
		}
		default:
			break;
	}
}

void Game::Update()
{
	manager->Refresh();

	manager->Update();

	newCamera.x = Player.GetComponent<TransformComponent>().position.x - cameraOffset.x;
	newCamera.y = Player.GetComponent<TransformComponent>().position.y - cameraOffset.y;
	camera = Vector2D::Lerp(camera, newCamera, 3.5f * DELTA_TIME);

	UpdateCollisions();
	UpdateUICollisions();
}

void Game::UpdateCollisions() 
{
	for (auto& [key, entity] : manager->entities)
	{
		Entity* entityA = entity.get();

		//Disabled Guard Clause
		if (!entityA->isActive()) 
		{
			continue;
		}

		if (entityA->HasComponent<ColliderComponent>() && !entityA->GetComponent<ColliderComponent>().IsTrigger())
		{
			ColliderComponent* colliderA = &entityA->GetComponent<ColliderComponent>();
			for (auto& [_key, _entity] : manager->entities)
			{
				Entity* entityB = _entity.get();
				
				//Disabled Guard Clause
				if (!entityB->isActive())
				{
					continue;
				}

				if (entityB->HasComponent<ColliderComponent>())
				{
					ColliderComponent* colliderB = &entityB->GetComponent<ColliderComponent>();

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
							entityA->GetComponent<TransformComponent>().dPosition.x -= collisionPoint.normal.x;
							entityA->GetComponent<TransformComponent>().dPosition.y -= collisionPoint.normal.y;
						}
						else 
						{
							entityB->GetComponent<ColliderComponent>().OnTrigger(entityA);
						}
					}
				}
			}
		}
	}
}

void Game::UpdateUICollisions()
{
	for (auto& [key, entity] : manager->entities)
	{
		Entity* entityA = entity.get();

		//Disabled Guard Clause
		if (!entityA->isActive())
		{
			continue;
		}

		if (entityA->HasComponent<UIColliderComponent>())
		{
			UIColliderComponent* colliderA = &entityA->GetComponent<UIColliderComponent>();

			colliderA->colliding = false;

			//If this gets laggy can do a sphere check before GJK algorithm
			if (collision.GJK(colliderA, &mousePos))
			{
				colliderA->colliding = true;
			}
		}
	}
}

void Game::Render()
{
	SDL_RenderClear(renderer);

	manager->Render();

	SDL_RenderPresent(renderer);
}

void Game::Clean()
{
	assets->~AssetManager();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}
