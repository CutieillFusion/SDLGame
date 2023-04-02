#pragma once
#include "Globals.h"
#include "Pokemon.h"
#include "ECS.h"

#define MOVE_DELAY 0.75f

struct BattleMove
{
	Pokemon* pokemonFrom;
	PokemonMove* move;
	Pokemon* pokemonTo;

	BattleMove(Pokemon* pokemonFrom, PokemonMove* move, Pokemon* pokemonTo);
};

class PokemonBattleManager
{
public:
	PokemonBattleManager();
	~PokemonBattleManager();

	std::vector<Pokemon*> playerTeam;
	std::vector<Pokemon*> enemyTeam;

	std::map<std::string, Entity*> uiEntities;

	std::vector<BattleMove> moveQue;

	//Used for the Wrapper Functions
	void AddMoveToQue(Pokemon* pokemonFrom, PokemonMove* move, Pokemon* pokemonTo);
	void SwapPokemon(int index);
	void UseMoveFromQue();

	void CalculateEnemyMove(Pokemon* enemyPokemon, Pokemon* playerPokemon);
	void CalculateMoveOrder();

	void UpdateUI();
	void Update();

	float moveDelay;
	bool turnStarted;
private:
	int CalculateDamage(Pokemon* pokemonFrom, PokemonMove* move, Pokemon* pokemonTo);
	void UpdateStats(Pokemon* pokemon, Entity* nameText, Entity* levelText, Entity* healthBar, Entity* healthText);
	void UpdateMoves(Pokemon* pokemon, Entity* moveImage, Entity* moveNameText, Entity* movePPText, int moveIndex);
	void UpdateTeam(Pokemon* pokemon, Entity* pokeballImage, int index);
};

namespace BattleManager 
{
	//Wrapper Functions
	void UseMove(std::vector<std::any> data);
	void SetPlayerTeam(std::vector<Pokemon*> team);
	void SetEnemyTeam(std::vector<Pokemon*> team);
	void AddUIEntity(std::string id, Entity* entity);
	void SwapPokemon(std::vector<std::any> data);

	void SetSwapMove(PokemonMove* swapMove);
}

