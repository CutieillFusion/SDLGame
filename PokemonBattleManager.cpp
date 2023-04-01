#include "PokemonBattleManager.h"
#include "Globals.h"
#include "SpriteComponent.h"
#include "SpriteRendererComponent.h"
#include "TextRendererComponent.h"
#include "RectComponent.h"
#include "ButtonComponent.h"

static PokemonBattleManager* singleton = nullptr;

PokemonBattleManager::PokemonBattleManager()
{
	if (singleton == nullptr) 
	{
		singleton = this;
	}
	else 
	{
		std::cout << "MULTIPLE POKEMON BATTLE MANAGERS" << std::endl;
	}
}

void PokemonBattleManager::UseMove(Pokemon* pokemon, PokemonMove* move)
{
	if (move->powerPoint <= 0 || enemyTurn) 
	{
		return;
	}

	int damage = CalculateDamage(pokemon, move, enemyTeam[0]);

	move->powerPoint--;

	enemyTeam[0]->health -= damage;
	
	if (enemyTeam[0]->health < 0)
	{
		enemyTeam[0]->health = 0;
	}
	
	turnDelay = 0.75f;
	enemyTurn = true;

	UpdateUI();
}

void PokemonBattleManager::EnemyAttack(Pokemon* pokemon)
{
	playerTeam[0]->health -= CalculateDamage(pokemon, pokemon->moves[1], playerTeam[0]);

	pokemon->moves[1]->powerPoint--;

	if (playerTeam[0]->health < 0)
	{
		playerTeam[0]->health = 0;
	}

	enemyTurn = false;
}

int PokemonBattleManager::CalculateDamage(Pokemon* pokemonFrom, PokemonMove* move, Pokemon* pokemonTo)
{
	if (move->move->power == 0) 
	{
		return 0;
	}
	
	int damage = (((2 * pokemonFrom->level) / 5 + 2) * move->move->power * (pokemonFrom->stats.attack / pokemonTo->stats.defense) / 50 + 2);
	srand((unsigned)time(NULL));
	int damageRoll = 84 + (rand() % 17);
	return (damage * damageRoll) / 100;
}

void PokemonBattleManager::UpdateUI()
{
	Pokemon* playerPokemon = playerTeam[0];
	Pokemon* enemyPokemon = enemyTeam[0];

	//Update Pokemon Sprites
	uiEntities["PlayerPokemon"]->getComponent<SpriteComponent>().ClearSetTexture(playerPokemon->pokemonObject->spriteId);
	uiEntities["EnemyPokemon"]->getComponent<SpriteComponent>().ClearSetTexture(enemyPokemon->pokemonObject->spriteId);

	//Update Stats
	auto playerStats = uiEntities["PlayerStats"]->GetChildren();
	auto enemyStats = uiEntities["EnemyStats"]->GetChildren();
	UpdateStats(playerPokemon, playerStats[0], playerStats[1], playerStats[2], playerStats[3]);
	UpdateStats(enemyPokemon, enemyStats[0], enemyStats[1], enemyStats[2], enemyStats[3]);

	//Update Moves
	for (int i = 0; i < 4; i++)
	{
		std::string index = "MoveButton" + std::to_string(i);
		auto move = uiEntities[index]->GetChildren();
		UpdateMoves(playerPokemon, uiEntities[index], move[0], move[1], i);
	}
}

void PokemonBattleManager::Update()
{
	if (enemyTurn) 
	{
		turnDelay -= DELTA_TIME;
		if (turnDelay <= 0) 
		{
			EnemyAttack(enemyTeam[0]);
			UpdateUI();
		}
	}
}

void PokemonBattleManager::UpdateStats(Pokemon* pokemon, Entity* nameText, Entity* levelText, Entity* healthBar, Entity* healthText) 
{
	nameText->getComponent<TextRendererComponent>().SetText(pokemon->name);
	levelText->getComponent<TextRendererComponent>().SetText("lvl: " + std::to_string(pokemon->level));
	healthBar->getComponent<RectComponent>().scale.x = 504 * ((float)pokemon->health / (float)pokemon->stats.health);
	healthText->getComponent<TextRendererComponent>().SetText(std::to_string(pokemon->health) + "/" + std::to_string(pokemon->stats.health));
}

void PokemonBattleManager::UpdateMoves(Pokemon* pokemon, Entity* moveImage, Entity* moveNameText, Entity* movePPText, int moveIndex)
{
	moveImage->getComponent<ButtonComponent>().data = { pokemon, pokemon->moves[moveIndex] };

	if (pokemon->moves[moveIndex] != nullptr)
	{
		moveImage->SetActiveStatus(true);
		moveNameText->getComponent<TextRendererComponent>().SetText(pokemon->moves[moveIndex]->move->name);
		movePPText->getComponent<TextRendererComponent>().SetText(std::to_string(pokemon->moves[moveIndex]->powerPoint) + "/" + std::to_string(pokemon->moves[moveIndex]->move->maxPowerPoint));
	}
	else
	{
		moveImage->SetActiveStatus(false);
	}
}

void BattleManager::UseMove(std::vector<std::any> data)
{
	auto pokemon = std::any_cast<Pokemon*>(data[0]);
	auto move = std::any_cast<PokemonMove*>(data[1]);
	singleton->UseMove(pokemon, move);
}

void BattleManager::SetPlayerTeam(std::vector<Pokemon*> team)
{
	singleton->playerTeam = team;
}

void BattleManager::SetEnemyTeam(std::vector<Pokemon*> team)
{
	singleton->enemyTeam = team;
}

void BattleManager::AddUIEntity(std::string id, Entity* entity)
{
	singleton->uiEntities.emplace(id, entity);
}
