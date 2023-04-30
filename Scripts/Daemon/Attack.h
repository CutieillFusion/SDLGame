#pragma once

#include "Enums.h"
#include "Turn.h"
#include <iostream>
#include <queue>

namespace Daemon
{
	namespace Model
	{
		class Daemon;

		class AttackEffect
		{
		public:
			virtual int Apply(Attack& attack, Daemon& attacker, Daemon& defender, std::queue<TurnAction>& turnQueue) { return 0; }
			virtual ~AttackEffect() {}
		};

		struct AttackData
		{
			std::string nameKey;
			int power;
			Type type;
			int accuracy;
			bool special;
			bool status;
			int criticalRate;
			bool neverFails;
			int ppMax;
			int priority;
			AttackEffect* preEffect = nullptr;
			AttackEffect* postEffect = nullptr;
			AttackEffect* ifFails = nullptr;
			//std::vector<TurnActionType> animationOrder;
			//std::queue<View::Transformation> opAnimsAtk;
			//std::queue<View::Transformation> opAnimsDef;
			//std::queue<std::string> animations;
		};

		typedef struct AttackData AttackData;

		class Attack
		{
		public:
			virtual ~Attack();
			Attack(std::string nameKey, int power, Type type, int accuracy, bool special, bool status, int criticalRate, bool neverFails, int ppMax, int priority, AttackEffect* preEffect, AttackEffect* postEffect, AttackEffect* fails);
			Attack(AttackData const& data);

			static Attack* NewAtk(std::string name);

			void HealPP() 
			{
				pp = ppMax;
			}

			Type GetType() {
				return type;
			}

			int GetPP() {
				return pp;
			}

			int GetPPMax() {
				return ppMax;
			}

			//"atk" attacks the "def" daemon
			int AttackDaemon(Daemon& atk, Daemon& def, std::queue<TurnAction>& turnQueue, bool attacker);

			//std::string save();

			void SetPP(int PP) {
				this->pp = PP;
			}

			void SetPPMax(int PPMax) {
				this->ppMax = PPMax;
			}

			int GetPriority() {
				return this->priority;
			}

			std::string GetName() {
				return name;
			}

			//std::queue<View::Transformation> getOpAnimsAtk() const {
			//	return opAnimsAtk;
			//}

			//std::queue<View::Transformation> getOpAnimsDef() const {
			//	return opAnimsDef;
			//}

			//std::queue<std::string> getAnimations() const {
			//	return animations;
			//}

			// methods used by pre and post Effects
			void SetPower(int power) { this->power = power; }
			int GetAccuracy() { return this->accuracy; }
			void SetAccuracy(int accuracy) { this->accuracy = accuracy; }
			int GetPart() { return part; }
			void SetPart(int part) { this->part = part; }
			int GetHpLost() { return this->hpLost; }
		
		protected:
			std::string nameKey;
			std::string name;
			int power;
			int priority;
			int accuracy;
			Type type;
			bool special;
			bool status;
			int part = 0;
			int pp;
			int ppMax;
			/** The critical hit rate is 1/criticalRate */
			int criticalRate;
			bool neverFails;

			AttackEffect* preEffect;
			AttackEffect* postEffect;
			AttackEffect* failEffect;
			/**Variables used in preEffect and postEffect*/
			int hpLost = 0;

			static std::map<std::string, AttackData> attackList;
		};
	}
}

