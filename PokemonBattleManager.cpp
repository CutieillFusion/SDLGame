#include "PokemonBattleManager.h"
#include "Globals.h"
#include "SpriteComponent.h"
#include "SpriteRendererComponent.h"
#include "TextRendererComponent.h"
#include "RectComponent.h"
#include "ButtonComponent.h"
#include "ImageRendererComponent.h"

static PokemonBattleManager* singleton = nullptr;
static PokemonMove* swapPokemonMove = nullptr;

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

PokemonBattleManager::~PokemonBattleManager()
{
}

void PokemonBattleManager::AddMoveToQue(Pokemon* pokemonFrom, PokemonMove* move, Pokemon* pokemonTo)
{
	if (move->powerPoint <= 0)
	{
		return;
	}

	moveQue.emplace_back(BattleMove(pokemonFrom, move, pokemonTo));
}

void PokemonBattleManager::SwapPokemon(int index)
{
	if (index != 0 && index < singleton->playerTeam.size())
	{
		if (playerTeam[0]->health > 0) 
		{
			AddMoveToQue(playerTeam[0], swapPokemonMove, enemyTeam[0]);
		}

		std::swap(playerTeam[0], playerTeam[index]);
		
		UpdateUI();
	}
}

void PokemonBattleManager::UseMoveFromQue()
{
	moveQue[0].pokemonTo->health -= CalculateDamage(moveQue[0].pokemonFrom, moveQue[0].move, moveQue[0].pokemonTo);
	moveQue[0].move->powerPoint--;
	
	if (moveQue[0].pokemonTo->health < 0) 
	{
		moveQue[0].pokemonTo->health = 0;
	}

	moveQue.erase(moveQue.begin());

	moveDelay = MOVE_DELAY;

	if (moveQue.size() == 0) 
	{
		turnStarted = false;
	}

	UpdateUI();
}

void PokemonBattleManager::CalculateEnemyMove(Pokemon* enemyPokemon, Pokemon* playerPokemon)
{
	int highestDamage = INT_MIN;
	PokemonMove* bestMove = enemyPokemon->moves[0];
	for (auto move : enemyPokemon->moves) 
	{	
		if (move == nullptr) 
		{
			continue;
		}

		int damage = CalculateDamage(enemyPokemon, move, playerPokemon);
		if (damage > highestDamage) 
		{
			bestMove = move;
			highestDamage = damage;
		}
	}

	AddMoveToQue(enemyPokemon, bestMove, playerPokemon);
}

void PokemonBattleManager::CalculateMoveOrder()
{
	int i, j;
	int n = (int)moveQue.size();
	
	//This is techincally unneeded but future proof for doubles and triple battles
	for (i = 0; i < n - 1; i++) 
	{
		// Last i elements are already in place
		for (j = 0; j < n - i - 1; j++) 
		{
			if (moveQue[j].pokemonFrom->stats.speed < moveQue[j + 1].pokemonFrom->stats.speed)
			{
				std::swap(moveQue[j], moveQue[j + 1]);
			}
			else if(moveQue[j].pokemonFrom->stats.speed == moveQue[j + 1].pokemonFrom->stats.speed)
			{
				srand((unsigned)time(NULL));
				if (rand() % 2 == 1) 
				{
					std::swap(moveQue[j], moveQue[j + 1]);
				}
			}
		}
	}

	turnStarted = true;
	moveDelay = MOVE_DELAY;
}


int PokemonBattleManager::CalculateDamage(Pokemon* pokemonFrom, PokemonMove* move, Pokemon* pokemonTo)
{
	if (move->move->power == 0) 
	{
		return 0;
	}

	//Damage Roll
	srand((unsigned)time(NULL));
	float damageRoll = (84.0f  + (rand() % 17)) / 100.0f;

	float damage = (2.0f * (float)pokemonFrom->level / 5.0f + 2.0f) * move->move->power * ((float)pokemonFrom->stats.attack / (float)pokemonTo->stats.defense) / 50.0f + 2.0f;

	float type = 1.0f;//need a method for type charts
	float weather = 1.0f;//Not Programmed in yet
	float critical = (rand() % 17) == 16 ? 1.5f : 1.0f;// 1 / 16 chance
	float stab = pokemonFrom->pokemonObject->types[0] == move->move->type || pokemonFrom->pokemonObject->types[1] == move->move->type ? 1.5f : 1.0f;//need a method for this
	float other = 1.0f;

	float specialDamage = type * weather * critical * stab * other;

	return (float)(damage * specialDamage * damageRoll);
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

	//Sets Player Team UI
	for (int i = 0; i < 6; i++) 
	{
		if (i < playerTeam.size()) 
		{
			UpdateTeam(playerTeam[i], playerStats[i + 4], i);
		}
	}

	//Sets Enemy Team UI
	for (int i = 0; i < 6; i++)
	{
		if (i < enemyTeam.size())
		{
			UpdateTeam(enemyTeam[i], enemyStats[i + 4], i);
		}
	}
}

void PokemonBattleManager::Update()
{
	if (moveQue.size() == 1 && !turnStarted) 
	{
		CalculateEnemyMove(enemyTeam[0], playerTeam[0]);
		CalculateMoveOrder();
	}

	if (turnStarted) 
	{
		moveDelay -= DELTA_TIME;
		if (moveDelay <= 0) 
		{
			UseMoveFromQue();
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

void PokemonBattleManager::UpdateTeam(Pokemon* pokemon, Entity* pokeballImage, int index)
{
	pokeballImage->getComponent<ButtonComponent>().data = { pokemon, index };

	if (pokemon != nullptr) 
	{
		pokeballImage->getComponent<SpriteComponent>().ClearSetTexture({ pokemon->pokemonObject->spriteId, pokemon->pokemonObject->spriteId, pokemon->pokemonObject->spriteId });
	}
	else 
	{
		pokeballImage->getComponent<SpriteComponent>().ClearSetTexture({ "Pokeball_Team", "Pokeball_Team_Highlighted", "Pokeball_Team_Activated" });
	}
}

void BattleManager::UseMove(std::vector<std::any> data)
{
	auto pokemon = std::any_cast<Pokemon*>(data[0]);
	auto move = std::any_cast<PokemonMove*>(data[1]);

	if (singleton->moveQue.size() <= 0) 
	{
		singleton->AddMoveToQue(pokemon, move, singleton->enemyTeam[0]);
	}
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

void BattleManager::SwapPokemon(std::vector<std::any> data)
{
	auto index = std::any_cast<int>(data[1]);

	singleton->SwapPokemon(index);
}

void BattleManager::SetSwapMove(PokemonMove* swapMove)
{
	swapPokemonMove = swapMove;
}

BattleMove::BattleMove(Pokemon* pokemonFrom, PokemonMove* move, Pokemon* pokemonTo) : pokemonFrom(pokemonFrom), move(move), pokemonTo(pokemonTo)
{

}
