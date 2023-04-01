#include "Pokemon.h"

Pokemon::Pokemon(PokemonObject* pokemonObject, std::vector<PokemonMove*> moves, PokemonStats ivs, PokemonNature nature, int level) 
	: pokemonObject(pokemonObject), moves(moves), level(level), ivs(ivs), name(pokemonObject->name), nature(nature)
{
	stats.CalculateStats(pokemonObject->baseStats, ivs, level);
	health = stats.health;
}