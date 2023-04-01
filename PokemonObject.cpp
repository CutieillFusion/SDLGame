#include "PokemonObject.h"
#include "Globals.h"

PokemonMove::PokemonMove(PokemonMoveObject* move) : move(move)
{
	powerPoint = move->maxPowerPoint;
}

PokemonMove::PokemonMove(PokemonMoveObject* move, int powerPoint) : move(move), powerPoint(powerPoint)
{

}

PokemonMoveObject::PokemonMoveObject(std::string name, PokemonType type, int power, int maxPowerPoint, int accuracy) 
	: name(name), type(type), power(power), maxPowerPoint(maxPowerPoint), accuracy(accuracy)
{
}

PokemonStats::PokemonStats(int health, int attack, int defense, int specialAttack, int specialDefense, int speed) 
	: health(health), attack(attack), defense(defense), specialAttack(specialAttack), specialDefense(specialDefense), speed(speed)
{
}

PokemonStats::PokemonStats()
{
}

PokemonObject::PokemonObject(std::string name, std::vector<PokemonType> types, PokemonStats baseStats, std::string spriteId)
	: name(name), types(types), baseStats(baseStats), spriteId(spriteId)
{
}

PokemonObject::PokemonObject()
{
}
