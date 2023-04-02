#include "PokemonManager.h"
#include "Globals.h"

PokemonManager::PokemonManager()
{
}

PokemonManager::~PokemonManager()
{
}

PokemonMove* PokemonManager::MakeMove(std::string id, int powerPoint) 
{
	return new PokemonMove(GetMoveObject(id), powerPoint);
}

PokemonMove* PokemonManager::MakeMove(std::string id)
{
	return new PokemonMove(GetMoveObject(id));
}

Pokemon* PokemonManager::MakePokemon(std::string id, std::vector<PokemonMove*> moves, PokemonStats ivs, PokemonNature nature, int level)
{
	return new Pokemon(GetPokemonObject(id), moves, ivs, nature, level);
}

Pokemon* PokemonManager::MakePokemon(std::string id, std::vector<PokemonMove*> moves, PokemonStats ivs, int level)
{
	return new Pokemon(GetPokemonObject(id), moves, ivs, PokemonObject::GetRandomNature(), level);
}

Pokemon* PokemonManager::MakePokemon(std::string id, std::vector<PokemonMove*> moves, PokemonNature nature, int level)
{
	return new Pokemon(GetPokemonObject(id), moves, PokemonObject::GetRandomIVs(), nature, level);
}

Pokemon* PokemonManager::MakePokemon(std::string id, std::vector<PokemonMove*> moves, int level)
{
	return new Pokemon(GetPokemonObject(id), moves, PokemonObject::GetRandomIVs(), PokemonObject::GetRandomNature(), level);
}

void PokemonManager::AddMoveObject(std::string name, PokemonType type, int power, int maxPowerPoint, int accuracy)
{
	moves.emplace(name, new PokemonMoveObject(name, type, power, maxPowerPoint, accuracy));
}

PokemonMoveObject* PokemonManager::GetMoveObject(std::string id)
{
	return moves[id];
}

void PokemonManager::AddPokemonObject(std::string name, std::vector<PokemonType> types, PokemonStats baseStats, std::string spriteId)
{
	pokemons.emplace(name, new PokemonObject(name, types, baseStats, spriteId));
}

PokemonObject* PokemonManager::GetPokemonObject(std::string id)
{
	return pokemons[id];
}

