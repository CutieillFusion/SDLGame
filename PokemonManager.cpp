#include "PokemonManager.h"
#include "Globals.h"

PokemonManager::PokemonManager()
{
}

void PokemonManager::Initialize() 
{
	//Make Loading Moves able to happen from JSON File
	PokemonMove* tackle = new PokemonMove(GetMove("Tackle"), 28);
	PokemonMove* leer = new PokemonMove(GetMove("Leer"), 16);
	PokemonMove* falseSwipe = new PokemonMove(GetMove("False Swipe"));

	PokemonMove* _tackle = new PokemonMove(GetMove("Tackle"), 28);
	PokemonMove* _leer = new PokemonMove(GetMove("Leer"), 16);
	PokemonMove* _falseSwipe = new PokemonMove(GetMove("False Swipe"));

	//PokemonStats tempBaseStats = { 40, 80, 100, 30, 30, 20 };
	PokemonStats tempIVs = { 31, 23, 23, 23, 23, 23 };

	//PokemonObject* crungly = new PokemonObject("Crungly", { GRASS }, tempBaseStats, "Crungly");
	Pokemon* _crungly = new Pokemon(GetPokemonObject("Crungly"), { tackle, falseSwipe, leer, nullptr }, tempIVs, HARDLY, 28);
	playerPokemon = _crungly;

	//PokemonObject* watry = new PokemonObject("Watry", { WATER }, tempBaseStats, "Watry");
	Pokemon* _watry = new Pokemon(GetPokemonObject("Watry"), { _tackle, _falseSwipe, _leer, nullptr }, tempIVs, HARDLY, 37);
	enemyPokemon = _watry;
}

void PokemonManager::AddMove(std::string name, PokemonType type, int power, int maxPowerPoint, int accuracy)
{
	moves.emplace(name, new PokemonMoveObject(name, type, power, maxPowerPoint, accuracy));
}

PokemonMoveObject* PokemonManager::GetMove(std::string id)
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

