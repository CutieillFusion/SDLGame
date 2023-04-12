#include "Enums.h"

namespace Daemon 
{
    namespace Model
    {
        namespace ArrayTypes 
        {
            //Stolen from OpMon Archive
            const float arrTypes[][18] =
            {
                {LO_, HI_, LO_, AVG, AVG, LO_, HI_, LO_, LO_, LO_, LO_, NOT, LO_, LO_, HI_, AVG, AVG, LO_},
                {AVG, AVG, AVG, AVG, AVG, HI_, AVG, AVG, LO_, AVG, AVG, AVG, HI_, LO_, AVG, AVG, LO_, HI_},
                {AVG, AVG, HI_, LO_, LO_, HI_, LO_, HI_, AVG, AVG, LO_, AVG, AVG, AVG, AVG, AVG, AVG, AVG},
                {LO_, AVG, AVG, LO_, HI_, AVG, LO_, LO_, AVG, AVG, HI_, AVG, AVG, AVG, AVG, AVG, AVG, AVG},
                {LO_, AVG, AVG, AVG, LO_, AVG, AVG, AVG, AVG, AVG, AVG, AVG, AVG, AVG, HI_, AVG, AVG, LO_},
                {HI_, LO_, NOT, AVG, AVG, AVG, AVG, AVG, LO_, AVG, AVG, HI_, AVG, AVG, AVG, AVG, LO_, AVG},
                {LO_, AVG, AVG, HI_, AVG, LO_, LO_, LO_, LO_, AVG, LO_, AVG, AVG, HI_, HI_, AVG, AVG, AVG},
                {HI_, HI_, AVG, AVG, AVG, AVG, HI_, LO_, AVG, AVG, AVG, AVG, AVG, HI_, AVG, AVG, AVG, AVG},
                {AVG, LO_, AVG, AVG, AVG, AVG, HI_, AVG, AVG, AVG, LO_, AVG, AVG, HI_, LO_, AVG, AVG, HI_},
                {AVG, HI_, AVG, AVG, AVG, AVG, AVG, AVG, AVG, AVG, AVG, AVG, AVG, AVG, AVG, NOT, AVG, AVG},
                {AVG, AVG, AVG, LO_, LO_, AVG, HI_, HI_, HI_, AVG, LO_, HI_, AVG, AVG, LO_, AVG, AVG, HI_},
                {AVG, LO_, AVG, AVG, AVG, LO_, AVG, AVG, LO_, AVG, LO_, LO_, HI_, AVG, HI_, AVG, AVG, AVG},
                {AVG, LO_, AVG, AVG, AVG, AVG, AVG, AVG, HI_, AVG, AVG, AVG, LO_, AVG, AVG, HI_, HI_, LO_},
                {HI_, HI_, AVG, HI_, AVG, AVG, LO_, AVG, AVG, LO_, HI_, LO_, AVG, AVG, HI_, AVG, AVG, LO_},
                {AVG, AVG, AVG, HI_, NOT, AVG, AVG, HI_, AVG, AVG, HI_, LO_, AVG, LO_, AVG, AVG, AVG, AVG},
                {AVG, NOT, AVG, AVG, AVG, AVG, AVG, AVG, LO_, NOT, AVG, LO_, AVG, AVG, AVG, HI_, HI_, AVG},
                {AVG, HI_, AVG, AVG, AVG, HI_, AVG, AVG, HI_, AVG, AVG, AVG, NOT, AVG, AVG, LO_, LO_, AVG},
                {AVG, LO_, AVG, AVG, HI_, AVG, AVG, HI_, LO_, AVG, LO_, AVG, AVG, HI_, NOT, AVG, AVG, AVG}
            };

            float CalcEffectiveness(Type atk, Type def1, Type def2)
            {
                int atkI = (int)atk;
                int type1I = (int)def1;
                int type2I = (int)def2;
                if (def2 == Type::NOTHING)
                {
                    return arrTypes[type1I][atkI];
                }
                else
                {
                    return arrTypes[type1I][atkI] * arrTypes[type2I][atkI];
                }
            }
        }

        bool operator==(Side side, int integer)
        {
            return side == integer;
        }

        bool operator!=(Side side, int integer)
        {
            return !(side == integer);
        }

        bool operator>=(Side side, int integer)
        {
            return side >= integer;
        }

        bool operator>(Side side, int integer)
        {
            return side > integer;
        }

        bool operator<=(Side side, int integer)
        {
            return side <= integer;
        }

        bool operator<(Side side, int integer)
        {
            return side < integer;
        }

        int operator+(Side side, int integer)
        {
            return side + integer;
        }

        int operator-(Side side, int integer)
        {
            return side - integer;
        }

        Side GetSide(int id)
        {
            switch (id) {
            case -1:
                return Side::NO_MOVE;
            case 0:
                return Side::TO_DOWN;
            case 1:
                return Side::TO_RIGHT;
            case 2:
                return Side::TO_LEFT;
            case 3:
                return Side::TO_UP;
            case -2:
                return Side::STAY;
            default:
                return Side::NO_MOVE;
            }
        }
    }
}
