#pragma once

#include "Item.h"
#include "Evolution.h"

namespace Daemon 
{
	namespace Model 
	{
		namespace Evolutions 
		{
			class E_Item : public Evolution
			{
			public:
				E_Item(int evo, std::string const& itemID);
				bool CheckEvolve(Daemon const& toCheck) const;

			protected:
				const std::string itemID;
			};

			class E_Level : public Evolution
			{
			public:
				E_Level(int evo, int level);
				bool CheckEvolve(Daemon const& toCheck) const;

			private:
				int level;
			};
		}
	}
}