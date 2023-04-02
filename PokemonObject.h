#pragma once
#include "Globals.h"

enum PokemonType
{
	NORMAL,
	FIRE,
	WATER,
	GRASS
};

struct PokemonMoveObject
{
public:
	std::string name;
	PokemonType type;
	int power;
	int maxPowerPoint;
	int accuracy;
	MoveFunction func;
	std::vector<std::any> data;

	PokemonMoveObject(std::string name, PokemonType type, int power, int maxPowerPoint, int accuracy);
	PokemonMoveObject(std::string name, PokemonType type, int power, int maxPowerPoint, int accuracy, MoveFunction func);
	PokemonMoveObject(std::string name, PokemonType type, int power, int maxPowerPoint, int accuracy, MoveFunction func, std::vector<std::any> data);
	PokemonMoveObject(std::string name, PokemonType type, int power, int maxPowerPoint, int accuracy, std::vector<std::any> data);
};

class PokemonMove
{
public:
	PokemonMoveObject* move;
	int powerPoint;

	PokemonMove(PokemonMoveObject* move);
	PokemonMove(PokemonMoveObject* move, int powerPoint);
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

struct PokemonStats
{
	int health;
	int attack;
	int defense;
	int specialAttack;
	int specialDefense;
	int speed;

	PokemonStats();
	PokemonStats(int health, int attack, int defense, int specialAttack, int specialDefense, int speed);

	void CalculateStats(PokemonStats baseStats, PokemonStats ivs, int level)
	{
		health = CalculateHealth(baseStats.health, ivs.health, level);
		attack = CalculateStat(baseStats.attack, ivs.attack, level);
		defense = CalculateStat(baseStats.defense, ivs.defense, level);
		specialAttack = CalculateStat(baseStats.specialAttack, ivs.specialAttack, level);
		specialDefense = CalculateStat(baseStats.specialDefense, ivs.specialDefense, level);
		speed = CalculateStat(baseStats.speed, ivs.speed, level);
	}

	int CalculateHealth(int baseStat, int iv, int level) 
	{
		return ((2 * baseStat + iv) * level / 100) + level + 10;
	}

	int CalculateStat(int baseStat, int iv, int level) 
	{
		return ((2 * baseStat + iv) * level / 100) + 5;
	}
};

class PokemonObject
{
public:
	std::string name;
	std::vector<PokemonType> types;
	PokemonStats baseStats;

	std::string spriteId;

	PokemonObject();
	PokemonObject(std::string name, std::vector<PokemonType> types, PokemonStats baseStats, std::string spriteId);

	static PokemonNature GetRandomNature() 
	{
		srand((unsigned int)time(NULL));
		return (PokemonNature)(rand() % 25);
	}

	static PokemonStats GetRandomIVs() 
	{
		srand((unsigned int)time(NULL));
		return PokemonStats(rand() % 32, rand() % 32, rand() % 32, rand() % 32, rand() % 32, rand() % 32);
	}
};

