#pragma once
#include <string>
#include <vector>

struct PokemonMove
{
	std::string name;
	PokemonType type;
	float power;
	int powerPoint;
	int maxPowerPoint;
	float accuracy;
};

enum PokemonNature
{
	HARDLY,
	LONELY, 
	BRAVE,
	ADAMANT,
	NAUGHTY,
	BOLD,
	DOCILE,
	RELAXED,
	IMPISH,
	LAX,
	TIMID,
	HASTY,
	SERIOUS,
	JOLLY,
	NAIVE,
	MODEST,
	MILD,
	QUIET,
	BASHFUL,
	RASH,
	CALM,
	GENTLE,
	SASSY,
	CAREFUL,
	QUIRKY
};

enum PokemonElement
{
	FIRE,
	WATER,
	GRASS
};

struct PokemonType
{
	std::string name;
	PokemonElement type;
	std::vector<PokemonElement> immuneTo;
	std::vector<PokemonElement> weakTo;
	std::vector<PokemonElement> strongTo;
	std::vector<PokemonElement> immuneFrom;
	std::vector<PokemonElement> weakFrom;
	std::vector<PokemonElement> strongFrom;
};

class PokemonObject
{
public:
	std::string name;
	std::vector<PokemonObject*> evolutions;
	PokemonType* types[2];
	PokemonNature nature;
	PokemonMove moves[4];
	int level;
};

