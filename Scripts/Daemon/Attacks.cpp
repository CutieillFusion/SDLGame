#include "Attacks.h"

namespace Daemon
{
	namespace Model
	{
		namespace Attacks
		{
            ChangeStatEffect::ChangeStatEffect(Target target, Model::Stats stat, int coef)
                : target(target), stat(stat), coef(coef) 
            {
            }

 /*           ChangeStatEffect::ChangeStatEffect(nlohmann::json const& data)
                : target(data.at("target"))
                , stat(data.at("stat"))
                , coef(data.at("coef")) {
            }*/

            int ChangeStatEffect::Apply(Attack&, Daemon& attacker, Daemon& defender, std::queue<TurnAction>& turnQueue) 
            {
                std::map<Stats, int (Daemon::*)(int)> stat_to_method =
                {
                  {Stats::ACC, &Daemon::ChangeACC},
                  {Stats::ATK, &Daemon::ChangeATK},
                  {Stats::ATKSPE, &Daemon::ChangeATKSPE},
                  {Stats::DEF, &Daemon::ChangeDEF},
                  {Stats::DEFSPE, &Daemon::ChangeDEFSPE},
                  {Stats::EVA, &Daemon::ChangeEVA},
                  {Stats::SPE, &Daemon::ChangeSPE},
                };
                auto change_method = stat_to_method[stat];

                // TODO : Add dialog if stat is at its min/max

                TurnAction statMod;
                NewTurnAction(&statMod);

                if (target == Target::ATTACKER) {
                    (attacker.*change_method)(coef); // diff_value = attacker.changeXXX(coef)
                    statMod.type = TurnActionType::ATK_STAT_MOD;
                }
                else {
                    (defender.*change_method)(coef); // diff_value = defender.changeXXX(coef)
                    statMod.type = TurnActionType::DEF_STAT_MOD;
                }
                statMod.statCoef = coef;
                statMod.statMod = stat;
                turnQueue.push(statMod);

                return 0;
            }

            int ProtectEffect::Apply(Attack& attack, Daemon& attacker, Daemon& defender, std::queue<TurnAction>& turnQueue)
            {
                //THINK OF THE LOGIC
                attacker.SetOtherMisc(0);
                return 0;
            }

            int HealEffect::Apply(Attack& attack, Daemon& attacker, Daemon& defender, std::queue<TurnAction>& turnQueue)
            {
                attacker.Heal(hp);

                std::cout << "Healed " << hp << std::endl;
                return 0;
            }
        }
	}
}