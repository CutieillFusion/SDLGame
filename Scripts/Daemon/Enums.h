#pragma once

#define NOT 0		//Not Effective at all
#define VLO 0.25	//Very Low Effectiveness
#define LO_ 0.5		//Low Effectiveness
#define AVG 1		//Average Effectiveness
#define HI_ 2		//High Effectiveness
#define VHI 4		//Very High Effectiveness

namespace Daemon 
{
	namespace Model
	{
		enum class Status
		{
			BURNING,
			PARALYSED,
			SLEEPING,
			FROZEN,
			POISONED,
			NOTHING
		};

		enum class Stats : int
		{
			ATK = 6,
			DEF = 1,
			ATKSPE = 2,
			DEFSPE = 3,
			SPE = 4,
			HP = 5,
			ACC = 7,
			EVA = 8,
			NOTHING = 0
		};

		enum class BagCategories
		{
			HEAL,
			OBJECTS,
			RARES,
			HMS
		};

		enum class Type : int 
		{
			LIQUID = 3,
			BURNING = 6,
			VEGETAL = 10,
			ELECTRON = 4,
			MENTAL = 12,
			BAD = 16,
			DRAGON = 2,
			FIGHT = 1,
			MINERAL = 13,
			GROUND = 14,
			NEUTRAL = 9,
			MAGIC = 5,
			GHOST = 15,
			TOXIC = 11,
			METAL = 0,
			BUG = 8,
			SKY = 17,
			COLD = 7,
			NOTHING = -1
		};

		enum class Side : int
		{
			TO_DOWN = 0,
			TO_RIGHT = 1,
			TO_LEFT = 2,
			TO_UP = 3,
			NO_MOVE = -1,
			STAY = -2
		};

		bool operator==(Side side, int integer);
		bool operator!=(Side side, int integer);
		bool operator>=(Side side, int integer);
		bool operator>(Side side, int integer);
		bool operator<=(Side side, int integer);
		bool operator<(Side side, int integer);
		int operator+(Side side, int integer);
		int operator-(Side side, int integer);

		Side GetSide(int);
		
		namespace ArrayTypes 
		{
			float CalcEffectiveness(Type atk, Type def1, Type def2);
		}
	}
}
