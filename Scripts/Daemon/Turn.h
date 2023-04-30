#pragma once

#include "Item.h"
#include <map>

namespace Daemon
{
	namespace Model
	{
		class Daemon;
		class Attack;

		enum class TurnType
		{
			ATTACK,
			ITEM,
			RUN,
			CHANGE
		};

		enum class TurnActionType : unsigned int
		{
			NOTHING = 0,
			ATK_UPDATE_HBAR = 1,
			ATK_USE_ITEM = 2,
			ATK_STAT_MOD = 3,
			ATK_DAEMON_CBACK = 4,
			ATK_DAEMON_OUT = 5,
			DIALOG = 6,
			DEF_UPDATE_HBAR = 7,
			DEF_USE_ITEM = 8,
			DEF_STAT_MOD = 9,
			DEF_DAEMON_CBACK = 10,
			DEF_DAEMON_OUT = 11,
			VICTORY = 12,
			DEFEAT = 13,
			RUN = 14,
			ATK_MOVE = 15,
			DEF_MOVE = 16,
			ANIMATION = 17,
			OPANIM = 18,
			NEXT = 19
		};

		struct TurnAction
		{
			int hpLost;
			std::string dialog;
			TurnActionType type;
			int statCoef;
			Stats statMod;
		};

		struct TurnData
		{
			Attack* attackUsed;
			Daemon* daemon;
			TurnType type;
			Item* itemUsed;
		};

		typedef struct TurnAction TurnAction;
		typedef struct TurnData TurnData;

		void NewTurnAction(TurnAction* toNew);
		void NewTurnData(TurnData* toNew);
		TurnAction CreateTurnDialogAction(std::string dialog);
	}
}

