#include "Nature.h"
namespace Daemon 
{
    namespace Model 
    {
        //I know this looks Gross but IDGAF
        const NatureClass BOLD = NatureClass(Stats::DEF, Stats::ATK, 0);
        const NatureClass BASHFUL = NatureClass(Stats::NOTHING, Stats::NOTHING, 1);
        const NatureClass BRAVE = NatureClass(Stats::ATK, Stats::SPE, 2);
        const NatureClass CALM = NatureClass(Stats::DEFSPE, Stats::ATK, 3);
        const NatureClass QUIET = NatureClass(Stats::ATKSPE, Stats::SPE, 4);
        const NatureClass DOCILE = NatureClass(Stats::NOTHING, Stats::NOTHING, 5);
        const NatureClass MILD = NatureClass(Stats::ATKSPE, Stats::DEF, 6);
        const NatureClass RASH = NatureClass(Stats::ATKSPE, Stats::DEFSPE, 7);
        const NatureClass GENTLE = NatureClass(Stats::DEFSPE, Stats::DEF, 8);
        const NatureClass HARDY = NatureClass(Stats::NOTHING, Stats::NOTHING, 9);
        const NatureClass JOLLY = NatureClass(Stats::SPE, Stats::ATKSPE, 10);
        const NatureClass LAX = NatureClass(Stats::DEF, Stats::DEFSPE, 11);
        const NatureClass IMPISH = NatureClass(Stats::DEF, Stats::ATKSPE, 12);
        const NatureClass SASSY = NatureClass(Stats::DEFSPE, Stats::SPE, 13);
        const NatureClass NAUGHTY = NatureClass(Stats::ATK, Stats::DEFSPE, 14);
        const NatureClass MODEST = NatureClass(Stats::ATKSPE, Stats::ATK, 15);
        const NatureClass NAIVE = NatureClass(Stats::SPE, Stats::DEFSPE, 16);
        const NatureClass HASTY = NatureClass(Stats::SPE, Stats::DEF, 17);
        const NatureClass CAREFUL = NatureClass(Stats::DEFSPE, Stats::ATKSPE, 18);
        const NatureClass QUIRKY = NatureClass(Stats::NOTHING, Stats::NOTHING, 19);
        const NatureClass RELAXED = NatureClass(Stats::DEF, Stats::SPE, 20);
        const NatureClass ADAMANT = NatureClass(Stats::ATK, Stats::ATKSPE, 21);
        const NatureClass SERIOUS = NatureClass(Stats::NOTHING, Stats::NOTHING, 22);
        const NatureClass LONELY = NatureClass(Stats::ATK, Stats::DEF, 23);
        const NatureClass TIMID = NatureClass(Stats::SPE, Stats::ATK, 24);

        const NatureClass natures[25] = { 
            BOLD,
            BASHFUL, 
            BRAVE, 
            CALM,
            QUIET,
            DOCILE,
            MILD, 
            RASH, 
            GENTLE, 
            HARDY, 
            JOLLY, 
            LAX, 
            IMPISH, 
            SASSY, 
            NAUGHTY, 
            MODEST, 
            NAIVE, 
            HASTY, 
            CAREFUL, 
            QUIRKY,
            RELAXED, 
            ADAMANT, 
            SERIOUS, 
            LONELY, 
            TIMID 
        };

        bool operator==(NatureClass const& one, NatureClass const& two)
        {
            return ((one.negative == two.negative) && (one.positive == two.positive));
        }
    }
}
