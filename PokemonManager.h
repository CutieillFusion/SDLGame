#pragma once

#include "Pokemon.h"
#include "Globals.h"

class PokemonManager
{
public:
	PokemonManager();
	~PokemonManager();

	Pokemon* playerPokemon;
	Pokemon* enemyPokemon;

	//Object Management
	void AddMoveObject(std::string name, PokemonType type, int power, int maxPowerPoint, int accuracy);
	PokemonMoveObject* GetMoveObject(std::string id);

	void AddPokemonObject(std::string name, std::vector<PokemonType> types, PokemonStats baseStats, std::string spriteId);
	PokemonObject* GetPokemonObject(std::string id);

	//Maker Functions
	PokemonMove* MakeMove(std::string id, int powerPoints);
	PokemonMove* MakeMove(std::string id);
	Pokemon* MakePokemon(std::string id, std::vector<PokemonMove*> moves, PokemonStats ivs, PokemonNature nature, int level);
	Pokemon* MakePokemon(std::string id, std::vector<PokemonMove*> moves, PokemonStats ivs, int level);
	Pokemon* MakePokemon(std::string id, std::vector<PokemonMove*> moves, PokemonNature nature, int level);
	Pokemon* MakePokemon(std::string id, std::vector<PokemonMove*> moves, int level);

private:
	std::map<std::string, PokemonMoveObject*> moves;
	std::map<std::string, PokemonObject*> pokemons;
};
