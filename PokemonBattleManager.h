#pragma once
#include "Globals.h"
#include "Pokemon.h"
#include "ECS.h"

class PokemonBattleManager
{
public:
	PokemonBattleManager();

	std::vector<Pokemon*> playerTeam;
	std::vector<Pokemon*> enemyTeam;

	std::map<std::string, Entity*> uiEntities;

	//Used for the Wrapper Functions
	void UseMove(Pokemon* pokemon, PokemonMove* move);
	void EnemyAttack(Pokemon* pokemon);

	void UpdateUI();
	void Update();
private:
	int CalculateDamage(Pokemon* pokemonFrom, PokemonMove* move, Pokemon* pokemonTo);
	void UpdateStats(Pokemon* pokemon, Entity* nameText, Entity* levelText, Entity* healthBar, Entity* healthText);
	void UpdateMoves(Pokemon* pokemon, Entity* moveImage, Entity* moveNameText, Entity* movePPText, int moveIndex);

	float turnDelay;
	bool enemyTurn;
};

namespace BattleManager 
{
	//Wrapper Functions
	void UseMove(std::vector<std::any> data);
	void SetPlayerTeam(std::vector<Pokemon*> team);
	void SetEnemyTeam(std::vector<Pokemon*> team);
	void AddUIEntity(std::string id, Entity* entity);
}

