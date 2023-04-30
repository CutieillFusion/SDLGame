#pragma once

#include "Attack.h"
#include "Daemon.h"
#include <cmath>
#include <iostream>

namespace Daemon
{
	namespace Model 
	{
        namespace Attacks
        {
            class ChangeStatEffect : public AttackEffect {
            public:
                enum class Target : int
                {
                    ATTACKER = 0,
                    DEFENDER = 1
                };

                ChangeStatEffect(Target target, Model::Stats stat, int coef);
                //ChangeStatEffect(nlohmann::json const& data);
                int Apply(Attack& attack, Daemon& attacker, Daemon& defender, std::queue<TurnAction>& turnQueue) override;

            protected:
                Target target;
                Model::Stats stat;
                int coef;
            };

            class ProtectEffect : public AttackEffect
            {
            public:
                ProtectEffect() { };
                int Apply(Attack& attack, Daemon& attacker, Daemon& defender, std::queue<TurnAction>& turnQueue) override;
            };

            class HealEffect : public AttackEffect
            {
            public:
                HealEffect(int hp) : hp(hp) { };
                int Apply(Attack& attack, Daemon& attacker, Daemon& defender, std::queue<TurnAction>& turnQueue) override;

            private:
                int hp;
            };
        }
	}
}

