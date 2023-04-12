#pragma once
#include "Enums.h"

namespace Daemon 
{
	namespace Model 
	{
		class NatureClass
		{
        public:
			Stats positive;
			Stats negative;

			int id;
            NatureClass(Stats positive, Stats negative, int id) : positive(positive), negative(negative), id(id) { };
		};

		extern const NatureClass natures[25];

		bool operator==(NatureClass const& one, NatureClass const& two);

        enum class Nature : unsigned int {
            BOLD = 0,
            BASHFUL = 1,
            BRAVE = 2,
            CALM = 3,
            QUIET = 4,
            DOCILE = 5,
            MILD = 6,
            RASH = 7,
            GENTLE = 8,
            HARDY = 9,
            JOLLY = 10,
            LAX = 11,
            IMPISH = 12,
            SASSY = 13,
            NAUGHTY = 14,
            MODEST = 15,
            NAIVE = 16,
            HASTY = 17,
            CAREFUL = 18,
            QUIRKY = 19,
            RELAXED = 20,
            ADAMANT = 21,
            SERIOUS = 22,
            LONELY = 23,
            TIMID = 24
        };
	}
}

