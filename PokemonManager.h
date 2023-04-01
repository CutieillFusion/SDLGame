#pragma once

#include "Pokemon.h"
#include "Globals.h"

class PokemonManager
{
public:
	PokemonManager();

	void Initialize();
	
	Pokemon* playerPokemon;
	Pokemon* enemyPokemon;

	//Object Management
	void AddMove(std::string name, PokemonType type, int power, int maxPowerPoint, int accuracy);
	PokemonMoveObject* GetMove(std::string id);

	void AddPokemonObject(std::string name, std::vector<PokemonType> types, PokemonStats baseStats, std::string spriteId);
	PokemonObject* GetPokemonObject(std::string id);

private:
	std::map<std::string, PokemonMoveObject*> moves;
	std::map<std::string, PokemonObject*> pokemons;
};
