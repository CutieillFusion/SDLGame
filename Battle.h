#pragma once

#include "ECS.h"
#include "Daemon.h"
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
			Battle(Daemon* atk, Daemon* def);
			void InitalizeUI(std::vector<std::any> ui);

			void Turn();
			bool CanAttack(Daemon* daemon, TurnData* daemonTurn);
			TurnData* AITurn();
			bool CheckBattleEnded();
			void UpdateBattleUI();
			~Battle() { };

			TurnData atkTurn;
			TurnData defTurn;

		private:
			//DaemonTeam* playerTeam;
			//DaemonTeam* trainerTeam;
	
			std::queue<Model::TurnAction> actionsQueue;

			/*The player's current OpMon*/
			Daemon* atk;
			/*The opposite trainer's current OpMon*/
			Daemon* def;

			bool atkFirst;

			bool sameAtk = false;
			bool sameDef = false;
			bool captured = false;

			int oldStats[2][5];
			Model::Type oldTypes[2][2];
			const Model::Species* oldSpecies[2];
			std::vector<Model::Attack*> oldAttacks[2];

			std::map<std::string, Entity*> ui;
		};
	}
}

