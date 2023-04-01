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
#include "JSONParser.h"
#include "UIColliderComponent.h"
#include "ButtonComponent.h"
#include "ButtonEvents.h"
#include "AssetManager.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;

Vector2D Game::camera = Vector2D(0, 0);
Vector2D cameraOffset = Vector2D(800.0f/(WORLD_SCALE * 2), 640.0f/(WORLD_SCALE * 2));//Center Screen

AssetManager* Game::assets = new AssetManager();

CollisionDetection collision = CollisionDetection();

Manager* Game::manager = new Manager();

//Entities
#pragma region ENTITIES
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
#pragma endregion

Game::Game()
{
} 

Game::~Game()
{
}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
	screenSize = Vector2D((float)width, (float)height);

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

	assets->AddJSONFile("Sprites", "Assets/JSON/Sprites.json");
	assets->AddJSONFile("PokemonMoves", "Assets/JSON/PokemonMoves.json");
	assets->AddJSONFile("Pokemons", "Assets/JSON/Pokemons.json");

	assets->LoadSpritesFromJSONFile("Sprites", "WorldSprites");
	assets->LoadSpritesFromJSONFile("Sprites", "PlayerSprites");
	assets->LoadSpritesFromJSONFile("Sprites", "PokemonSprites");
	assets->LoadSpritesFromJSONFile("Sprites", "UISprites");
	assets->LoadMovesFromJSONFile("PokemonMoves", "Moves");
	assets->LoadPokemonsFromJSONFile("Pokemons", "Pokemons");

	assets->AddFont("8Bit36", "Assets/Fonts/8Bit.ttf", 36);
	assets->AddFont("8Bit56", "Assets/Fonts/8Bit.ttf", 56);
	assets->AddFont("Test", "Assets/Fonts/Test.ttf", 32);

	assets->pokemonManager->Initialize();

	//map = new Map(manager);

	std::vector<Vector3D> boxCollider =
	{
		Vector3D(0.0f, 0.0f, 0.0f),
		Vector3D(1.0f, 0.0f, 0.0f),
		Vector3D(1.0f, 1.0f, 0.0f),
		Vector3D(0.0f, 1.0f, 0.0f)
	};

	SDL_Color textColor = { 0, 0, 0, 255 };

	Background.addComponent<RectComponent>(Vector3D(0, 0, 0), Vector3D(screenSize.x, screenSize.y, 0));
	Background.addComponent<SpriteComponent>("Grass");
	Background.addComponent<ImageRendererComponent>();

	#pragma region ITEM_SLOTS
	std::vector<std::string> itemSlotIds =
	{
		"ItemSlot", "ItemSlot_Highlighted", "ItemSlot_Activated"
	};

	Vector3D itemSlotScale = Vector3D(screenSize.x / 12.0f, screenSize.y * 4.0f / 27.0f, 0);
	ItemSlot0.addComponent<RectComponent>(Vector3D(screenSize.x * 8.0f / 12.0f, screenSize.y * 23.0f / 27.0f, 0), itemSlotScale);
	ItemSlot0.addComponent<SpriteComponent>(itemSlotIds);
	ItemSlot0.addComponent<ImageRendererComponent>();
	ItemSlot0.addComponent<UIColliderComponent>(boxCollider);
	ItemSlot0.addComponent<ButtonComponent>();

	ItemSlot1.addComponent<RectComponent>(Vector3D(screenSize.x * 1.0f / 12.0f, 0, 0), itemSlotScale);
	ItemSlot1.addComponent<SpriteComponent>(itemSlotIds);
	ItemSlot1.addComponent<ImageRendererComponent>();
	ItemSlot1.addComponent<UIColliderComponent>(boxCollider);
	ItemSlot1.addComponent<ButtonComponent>();
	ItemSlot1.SetParent(&ItemSlot0);

	ItemSlot2.addComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 12.0f, 0, 0), itemSlotScale);
	ItemSlot2.addComponent<SpriteComponent>(itemSlotIds);
	ItemSlot2.addComponent<ImageRendererComponent>();
	ItemSlot2.addComponent<UIColliderComponent>(boxCollider);
	ItemSlot2.addComponent<ButtonComponent>();
	ItemSlot2.SetParent(&ItemSlot0);

	ItemSlot3.addComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 12.0f, 0, 0), itemSlotScale);
	ItemSlot3.addComponent<SpriteComponent>(itemSlotIds);
	ItemSlot3.addComponent<ImageRendererComponent>();
	ItemSlot3.addComponent<UIColliderComponent>(boxCollider);
	ItemSlot3.addComponent<ButtonComponent>();
	ItemSlot3.SetParent(&ItemSlot0);
	#pragma endregion

	#pragma region POKEMON_PLATFORMS
	Vector3D pokemonPlatformScale = Vector3D(384, screenSize.y * 16.0f / 45.0f, 0);
	Vector3D pokemonScale = Vector3D(screenSize.x * 9.0f / 50.0f, screenSize.y * 8.0f / 25.0f, 0);

	PlayerPokemonPlatform.addComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 80.0f, screenSize.y * 2.0f / 3.0f, 0), pokemonPlatformScale);
	PlayerPokemonPlatform.addComponent<SpriteComponent>("Pokemon_Platform");
	PlayerPokemonPlatform.addComponent<ImageRendererComponent>();

	PlayerPokemon.addComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 50.0f, screenSize.y * 128.0f / 225.0f, 0), pokemonScale);
	PlayerPokemon.addComponent<SpriteComponent>("Pokemon_0");
	PlayerPokemon.addComponent<ImageRendererComponent>();

	EnemyPokemonPlatform.addComponent<RectComponent>(Vector3D(screenSize.x * 43.0f / 100.0f, screenSize.y / 9.0f, 0), pokemonPlatformScale);
	EnemyPokemonPlatform.addComponent<SpriteComponent>("Pokemon_Platform");
	EnemyPokemonPlatform.addComponent<ImageRendererComponent>();

	EnemyPokemon.addComponent<RectComponent>(Vector3D(screenSize.x * 47.0f / 100.0f, screenSize.y * 8.0f / 225.0f, 0), pokemonScale);
	EnemyPokemon.addComponent<SpriteComponent>("Pokemon_26");
	EnemyPokemon.addComponent<ImageRendererComponent>();
	#pragma endregion

	#pragma region PLAYER_STATS
	PlayerStatsBackground.addComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 10.0f, screenSize.y * 32.0f / 45.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, screenSize.y * 32.0f / 225.0f, 0));
	PlayerStatsBackground.addComponent<SpriteComponent>("Stats_Border");
	PlayerStatsBackground.addComponent<ImageRendererComponent>();

	PlayerStatsName.addComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 400.0f, screenSize.y * 1.0f / 225.0f, 0), Vector3D(screenSize.x * 2.0f / 25.0f, screenSize.y * 8.0f / 225.0f, 0));
	PlayerStatsName.addComponent<TextRendererComponent>("Name", "8Bit36", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	PlayerStatsName.SetParent(&PlayerStatsBackground);

	PlayerStatsLevel.addComponent<RectComponent>(Vector3D(-12, screenSize.y * 1.0f / 225.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, 32, 0));
	PlayerStatsLevel.addComponent<TextRendererComponent>("Lvl: 01", "8Bit36", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	PlayerStatsLevel.SetParent(&PlayerStatsBackground);

	PlayerStatsHealth.addComponent<RectComponent>(Vector3D(screenSize.x * 1.0f / 100.0f, screenSize.y * 4.0f / 75.0f, 0), Vector3D(504, 28, 0));
	PlayerStatsHealth.addComponent<SpriteComponent>("Health_Bar");
	PlayerStatsHealth.addComponent<ImageRendererComponent>();
	PlayerStatsHealth.SetParent(&PlayerStatsBackground);

	PlayerStatsHealthText.addComponent<RectComponent>(Vector3D(-24, screenSize.y * 22.0f / 225.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, 32, 0));
	PlayerStatsHealthText.addComponent<TextRendererComponent>("300/350", "8Bit36", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	PlayerStatsHealthText.SetParent(&PlayerStatsBackground);
	#pragma endregion

	#pragma region ENEMY_STATS
	EnemyStatsBackground.addComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 200.0f, screenSize.y * 2.0f / 75.0f, 0), Vector3D(screenSize.x * 17.0f / 50.f, screenSize.y * 32.0f / 225.0f, 0));
	EnemyStatsBackground.addComponent<SpriteComponent>("Stats_Border");
	EnemyStatsBackground.addComponent<ImageRendererComponent>();

	EnemyStatsName.addComponent<RectComponent>(Vector3D(screenSize.x * 3.0f / 400.0f, screenSize.y * 1.0f / 225.0f, 0), Vector3D(screenSize.x * 2.0f / 25.0f, screenSize.y * 8.0f / 225.0f, 0));
	EnemyStatsName.addComponent<TextRendererComponent>("Name", "8Bit36", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	EnemyStatsName.SetParent(&EnemyStatsBackground);

	EnemyStatsLevel.addComponent<RectComponent>(Vector3D(-12, screenSize.y * 1.0f / 225.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, screenSize.y * 8.0f / 225.0f, 0));
	EnemyStatsLevel.addComponent<TextRendererComponent>("Lvl: 01", "8Bit36", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	EnemyStatsLevel.SetParent(&EnemyStatsBackground);

	EnemyStatsHealth.addComponent<RectComponent>(Vector3D(screenSize.x * 1.0f / 100.0f, screenSize.y * 4.0f / 75.0f, 0), Vector3D(screenSize.x * 63.0f / 200.0f, screenSize.y * 7.0f / 225.0f, 0));
	EnemyStatsHealth.addComponent<SpriteComponent>("Health_Bar");
	EnemyStatsHealth.addComponent<ImageRendererComponent>();
	EnemyStatsHealth.SetParent(&EnemyStatsBackground);

	EnemyStatsHealthText.addComponent<RectComponent>(Vector3D(-24, screenSize.y * 22.0f / 225.0f, 0), Vector3D(screenSize.x * 17.0f / 50.0f, screenSize.y * 8.0f / 225.0f, 0));
	EnemyStatsHealthText.addComponent<TextRendererComponent>("300/350", "8Bit36", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	EnemyStatsHealthText.SetParent(&EnemyStatsBackground);
	#pragma endregion 

	#pragma region POKEMON_TEAM
	std::vector<std::string> pokeballIds =
	{
		"Pokeball_Team", "Pokeball_Team_Highlighted", "Pokeball_Team_Activated"
	};

	Vector3D pokeballScale = Vector3D(80, 80, 0);
	PlayerTeam0.addComponent<RectComponent>(Vector3D(0, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	PlayerTeam0.addComponent<SpriteComponent>(pokeballIds);
	PlayerTeam0.addComponent<ImageRendererComponent>();
	PlayerTeam0.addComponent<UIColliderComponent>(boxCollider);
	PlayerTeam0.addComponent<ButtonComponent>();
	PlayerTeam0.SetParent(&PlayerStatsBackground);

	PlayerTeam1.addComponent<RectComponent>(Vector3D(screenSize.x * 23.0f / 400.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	PlayerTeam1.addComponent<SpriteComponent>(pokeballIds);
	PlayerTeam1.addComponent<ImageRendererComponent>();
	PlayerTeam1.addComponent<UIColliderComponent>(boxCollider);
	PlayerTeam1.addComponent<ButtonComponent>();
	PlayerTeam1.SetParent(&PlayerStatsBackground);

	PlayerTeam2.addComponent<RectComponent>(Vector3D(screenSize.x * 23.0f / 200.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	PlayerTeam2.addComponent<SpriteComponent>(pokeballIds);
	PlayerTeam2.addComponent<ImageRendererComponent>();
	PlayerTeam2.addComponent<UIColliderComponent>(boxCollider);
	PlayerTeam2.addComponent<ButtonComponent>();
	PlayerTeam2.SetParent(&PlayerStatsBackground);

	PlayerTeam3.addComponent<RectComponent>(Vector3D(screenSize.x * 69.0f / 400.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	PlayerTeam3.addComponent<SpriteComponent>(pokeballIds);
	PlayerTeam3.addComponent<ImageRendererComponent>();
	PlayerTeam3.addComponent<UIColliderComponent>(boxCollider);
	PlayerTeam3.addComponent<ButtonComponent>();
	PlayerTeam3.SetParent(&PlayerStatsBackground);

	PlayerTeam4.addComponent<RectComponent>(Vector3D(screenSize.x * 23.0f / 100.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	PlayerTeam4.addComponent<SpriteComponent>(pokeballIds);
	PlayerTeam4.addComponent<ImageRendererComponent>();
	PlayerTeam4.addComponent<UIColliderComponent>(boxCollider);
	PlayerTeam4.addComponent<ButtonComponent>();
	PlayerTeam4.SetParent(&PlayerStatsBackground);

	PlayerTeam5.addComponent<RectComponent>(Vector3D(screenSize.x * 115.0f / 400.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	PlayerTeam5.addComponent<SpriteComponent>(pokeballIds);
	PlayerTeam5.addComponent<ImageRendererComponent>();
	PlayerTeam5.addComponent<UIColliderComponent>(boxCollider);
	PlayerTeam5.addComponent<ButtonComponent>();
	PlayerTeam5.SetParent(&PlayerStatsBackground);

	EnemyTeam0.addComponent<RectComponent>(Vector3D(0, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	EnemyTeam0.addComponent<SpriteComponent>(pokeballIds);
	EnemyTeam0.addComponent<ImageRendererComponent>();
	EnemyTeam0.addComponent<UIColliderComponent>(boxCollider);
	EnemyTeam0.addComponent<ButtonComponent>();
	EnemyTeam0.SetParent(&EnemyStatsBackground);

	EnemyTeam1.addComponent<RectComponent>(Vector3D(screenSize.x * 23.0f / 400.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	EnemyTeam1.addComponent<SpriteComponent>(pokeballIds);
	EnemyTeam1.addComponent<ImageRendererComponent>();
	EnemyTeam1.addComponent<UIColliderComponent>(boxCollider);
	EnemyTeam1.addComponent<ButtonComponent>();
	EnemyTeam1.SetParent(&EnemyStatsBackground);

	EnemyTeam2.addComponent<RectComponent>(Vector3D(screenSize.x * 23.0f / 200.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	EnemyTeam2.addComponent<SpriteComponent>(pokeballIds);
	EnemyTeam2.addComponent<ImageRendererComponent>();
	EnemyTeam2.addComponent<UIColliderComponent>(boxCollider);
	EnemyTeam2.addComponent<ButtonComponent>();
	EnemyTeam2.SetParent(&EnemyStatsBackground);

	EnemyTeam3.addComponent<RectComponent>(Vector3D(screenSize.x * 69.0f / 400.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	EnemyTeam3.addComponent<SpriteComponent>(pokeballIds);
	EnemyTeam3.addComponent<ImageRendererComponent>();
	EnemyTeam3.addComponent<UIColliderComponent>(boxCollider);
	EnemyTeam3.addComponent<ButtonComponent>();
	EnemyTeam3.SetParent(&EnemyStatsBackground);

	EnemyTeam4.addComponent<RectComponent>(Vector3D(screenSize.x * 23.0f / 100.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	EnemyTeam4.addComponent<SpriteComponent>(pokeballIds);
	EnemyTeam4.addComponent<ImageRendererComponent>();
	EnemyTeam4.addComponent<UIColliderComponent>(boxCollider);
	EnemyTeam4.addComponent<ButtonComponent>();
	EnemyTeam4.SetParent(&EnemyStatsBackground);

	EnemyTeam5.addComponent<RectComponent>(Vector3D(screenSize.x * 115.0f / 400.0f, screenSize.y * 4.0f / 25.0f, 0), pokeballScale);
	EnemyTeam5.addComponent<SpriteComponent>(pokeballIds);
	EnemyTeam5.addComponent<ImageRendererComponent>();
	EnemyTeam5.addComponent<UIColliderComponent>(boxCollider);
	EnemyTeam5.addComponent<ButtonComponent>();
	EnemyTeam5.SetParent(&EnemyStatsBackground);
	#pragma endregion

	#pragma region MOVE_BUTTONS
	std::vector<std::string> moveButton =
	{
		"Move_Normal", "Move_Normal_Highlighted", "Move_Normal_Activated"
	};
	Vector3D moveButtonSize = Vector3D(screenSize.x / 3.0f, screenSize.y * 1.0f / 15.0f + 1, 0);
	ButtonEvent useMove = BattleManager::UseMove;

	MoveButton0.addComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 500, 0), moveButtonSize);
	MoveButton0.addComponent<SpriteComponent>(moveButton);
	MoveButton0.addComponent<ImageRendererComponent>();
	MoveButton0.addComponent<UIColliderComponent>(boxCollider);
	MoveButton0.addComponent<ButtonComponent>(useMove);

	MoveButtonText0.addComponent<RectComponent>(Vector3D(12, 4, 0), moveButtonSize);
	MoveButtonText0.addComponent<TextRendererComponent>("Tackle", "8Bit56", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	MoveButtonText0.SetParent(&MoveButton0);

	MoveButtonTextPP0.addComponent<RectComponent>(Vector3D(0, 4, 0), moveButtonSize);
	MoveButtonTextPP0.addComponent<TextRendererComponent>("35/35", "8Bit56", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	MoveButtonTextPP0.SetParent(&MoveButton0);

	MoveButton1.addComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 560, 0), moveButtonSize);
	MoveButton1.addComponent<SpriteComponent>(moveButton);
	MoveButton1.addComponent<ImageRendererComponent>();
	MoveButton1.addComponent<UIColliderComponent>(boxCollider);
	MoveButton1.addComponent<ButtonComponent>(useMove);

	MoveButtonText1.addComponent<RectComponent>(Vector3D(12, 4, 0), moveButtonSize);
	MoveButtonText1.addComponent<TextRendererComponent>("Leer", "8Bit56", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	MoveButtonText1.SetParent(&MoveButton1);

	MoveButtonTextPP1.addComponent<RectComponent>(Vector3D(0, 4, 0), moveButtonSize);
	MoveButtonTextPP1.addComponent<TextRendererComponent>("30/30", "8Bit56", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	MoveButtonTextPP1.SetParent(&MoveButton1);

	MoveButton2.addComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 620, 0), moveButtonSize);
	MoveButton2.addComponent<SpriteComponent>(moveButton);
	MoveButton2.addComponent<ImageRendererComponent>();
	MoveButton2.addComponent<UIColliderComponent>(boxCollider);
	MoveButton2.addComponent<ButtonComponent>(useMove);

	MoveButtonText2.addComponent<RectComponent>(Vector3D(12, 4, 0), moveButtonSize);
	MoveButtonText2.addComponent<TextRendererComponent>("False Swipe", "8Bit56", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	MoveButtonText2.SetParent(&MoveButton2);

	MoveButtonTextPP2.addComponent<RectComponent>(Vector3D(0, 4, 0), moveButtonSize);
	MoveButtonTextPP2.addComponent<TextRendererComponent>("40/40", "8Bit56", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	MoveButtonTextPP2.SetParent(&MoveButton2);

	MoveButton3.addComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 680, 0), moveButtonSize);
	MoveButton3.addComponent<SpriteComponent>(moveButton);
	MoveButton3.addComponent<ImageRendererComponent>();
	MoveButton3.addComponent<UIColliderComponent>(boxCollider);
	MoveButton3.addComponent<ButtonComponent>(useMove);
	
	MoveButtonText3.addComponent<RectComponent>(Vector3D(12, 4, 0), moveButtonSize);
	MoveButtonText3.addComponent<TextRendererComponent>("Protect", "8Bit56", textColor, TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP);
	MoveButtonText3.SetParent(&MoveButton3);

	MoveButtonTextPP3.addComponent<RectComponent>(Vector3D(0, 4, 0), moveButtonSize);
	MoveButtonTextPP3.addComponent<TextRendererComponent>("10/10", "8Bit56", textColor, TEXT_ALIGN_RIGHT, TEXT_ALIGN_TOP);
	MoveButtonTextPP3.SetParent(&MoveButton3);
	#pragma endregion

	BattleTextBox.addComponent<RectComponent>(Vector3D(screenSize.x * 2.0f / 3.0f, 400, 0), Vector3D(screenSize.x / 3.0f, 80, 0));
	BattleTextBox.addComponent<SpriteComponent>("BattleTextBox");
	BattleTextBox.addComponent<ImageRendererComponent>(); 
	BattleTextBox.addComponent<TextRendererComponent>(" SELECT NEXT MOVE...", "8Bit56", textColor, TEXT_ALIGN_CENTER, TEXT_ALIGN_MIDDLE);

	//Sends Pokemon To BattleManager
	BattleManager::SetPlayerTeam({ assets->pokemonManager->playerPokemon });
	BattleManager::SetEnemyTeam({ assets->pokemonManager->enemyPokemon });

	//Send UI Entites to BattleManager
	BattleManager::AddUIEntity("PlayerPokemon", &PlayerPokemon);
	BattleManager::AddUIEntity("EnemyPokemon", &EnemyPokemon);

	BattleManager::AddUIEntity("PlayerStats", &PlayerStatsBackground);
	BattleManager::AddUIEntity("EnemyStats", &EnemyStatsBackground);

	BattleManager::AddUIEntity("MoveButton0", &MoveButton0);
	BattleManager::AddUIEntity("MoveButton1", &MoveButton1);
	BattleManager::AddUIEntity("MoveButton2", &MoveButton2);
	BattleManager::AddUIEntity("MoveButton3", &MoveButton3);

	//Initializing BattleManager UI
	assets->pokemonBattleManager->UpdateUI();
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

void Game::update()
{
	manager->Refresh();

	manager->Update();

	//newCamera.x = Player.getComponent<TransformComponent>().position.x - cameraOffset.x;
	//newCamera.y = Player.getComponent<TransformComponent>().position.y - cameraOffset.y;
	camera = Vector2D::Lerp(camera, newCamera, 3.5f * DELTA_TIME);

	assets->pokemonBattleManager->Update();

	UpdateCollisions();
	UpdateUICollisions();
}

void Game::UpdateCollisions() 
{
	for (auto &[key, entity] : manager->entities)
	{
		Entity* entityA = entity.get();

		//Disabled Guard Clause
		if (!entityA->isActive()) 
		{
			continue;
		}

		if (entityA->hasComponent<ColliderComponent>() && !entityA->getComponent<ColliderComponent>().IsTrigger())
		{
			ColliderComponent* colliderA = &entityA->getComponent<ColliderComponent>();
			for (auto& [_key, _entity] : manager->entities)
			{
				Entity* entityB = _entity.get();
				
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
	for (auto& [key, entity] : manager->entities)
	{
		Entity* entityA = entity.get();

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