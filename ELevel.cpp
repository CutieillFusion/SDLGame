#include "evolutions.h"
#include "Evolution.h"

namespace Daemon
{
	namespace Model
	{
		namespace Evolutions
		{
			E_Level::E_Level(int evo, int level) : Evolution(evo) 
			{
				this->level = level;
			}

			bool E_Level::CheckEvolve(Daemon const& toCheck) const 
			{
				return toCheck.GetLevel() >= level;
			}
		}
	}
}