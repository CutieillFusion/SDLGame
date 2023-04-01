#pragma once
#include "PokemonObject.h"
#include "Globals.h"

class Pokemon
{
public:
	std::string name;
	PokemonObject* pokemonObject;
	std::vector<PokemonMove*> moves;
	PokemonStats ivs;
	PokemonStats stats;
	PokemonNature nature;
	int level;
	int health;

	Pokemon(PokemonObject* pokemonObject, std::vector<PokemonMove*> moves, PokemonStats ivs, PokemonNature nature, int level);
};

