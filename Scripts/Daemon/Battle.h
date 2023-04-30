#pragma once

#include "..\Globals.h"
#include "..\ECS\ECS.h"
#include "Daemon.h"
#include "DaemonTeam.h"
#include "Turn.h"
#include "Attack.h"
#include "Attacks.h"

namespace Daemon
{
	namespace Model
	{
		class Battle
		{
		public:
			Battle(DaeTeam* playerTeam, DaeTeam* trainerTeam, int atkIndex, int defIndex);
			static void InitalizeUI(std::map<std::string, Entity*> _ui);

			bool CanAttack(Daemon* daemon, TurnData* daemonTurn);
			bool CheckTurnEnded();
			bool CheckBattleEnded();
			void EndBattle();
			void UpdateBattleUI();
			~Battle();

			void Turn();
			TurnData* AITurn();
			bool PlayerTurn(int moveIndex);
			bool SwapTurn(int daemonIndex);
			bool ItemTurn(Item* item);
			bool RunTurn();

			TurnData atkTurn;
			TurnData defTurn;

		private:
			DaeTeam* playerTeam;
			DaeTeam* trainerTeam;
	
			std::queue<Model::TurnAction> actionsQueue;

			/*The player's current Daemon*/
			Daemon* atk;
			/*The opposite trainer's current Daemon*/
			Daemon* def;

			bool atkFirst;

			bool sameAtk = false;
			bool sameDef = false;
			bool captured = false;

			int oldStats[2][5];
			Model::Type oldTypes[2][2];
			const Model::Species* oldSpecies[2];
			std::vector<Model::Attack*> oldAttacks[2];
		};

		namespace BattleWrapper
		{
			void UseMove(std::vector<std::any> data);
			void SwapDaemon(std::vector<std::any> data);

		}
	}
}

