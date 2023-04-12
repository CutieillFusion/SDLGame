#pragma once

#include "Daemon.h"
#include <iostream>

namespace Daemon 
{
	namespace Model 
	{
		class Species;

		class Evolution
		{
		public:
			Evolution()
			{
				evo = -1;
			}
			Evolution(int evo) : evo(evo) { };

			virtual bool CheckEvolve(Daemon const& toCheck) const = 0;
			Species* GetEvolution() const { return toEvolve; }

			void CheckEvo();

			virtual ~Evolution() { };

		protected:
			Species* toEvolve;
			int evo;
		};
	}
}

