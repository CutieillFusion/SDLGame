#include "evolutions.h"
#include "Evolution.h"

namespace Daemon
{
	namespace Model
	{
		namespace Evolutions
		{
			E_Item::E_Item(int evo, std::string const& itemId)
				: Evolution(evo)
				, itemID(itemId) {
			}

			bool E_Item::CheckEvolve(Daemon const& toCheck) const {
				//Temporary
				return false;
			}
		}
	}
}