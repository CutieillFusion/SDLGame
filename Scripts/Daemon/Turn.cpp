#include "Turn.h"
#include "Attack.h"
#include "Daemon.h"

namespace Daemon
{
	namespace Model
	{
		void NewTurnAction(TurnAction* toNew) 
		{
			toNew->hpLost = 0;
			toNew->dialog = std::string();
			toNew->type = TurnActionType::NOTHING;
			toNew->statCoef = 0;
		}

		void NewTurnData(TurnData* toNew) 
		{
			toNew->daemon = nullptr;
			toNew->attackUsed = nullptr;
			toNew->type = TurnType::ATTACK;
			toNew->itemUsed = nullptr;
		}

		TurnAction CreateTurnDialogAction(std::string dialog) 
		{
			TurnAction ta;
			NewTurnAction(&ta);
			ta.type = TurnActionType::DIALOG;
			ta.dialog = dialog;
			return ta;
		}
	}
}