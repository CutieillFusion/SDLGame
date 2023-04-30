#include "Attack.h"

namespace Daemon
{
	namespace Model
	{
        std::map<std::string, AttackData> Attack::attackList;

        Attack::Attack(std::string nameKey, int power, Type type, int accuracy, bool special, bool status, int criticalRate, bool neverFails, int ppMax, int priority, AttackEffect* preEffect, AttackEffect* postEffect, AttackEffect* fails)
            : nameKey(nameKey), name(nameKey), power(power), priority(priority), accuracy(accuracy), type(type), special(special), status(status), pp(ppMax), ppMax(ppMax)
            , criticalRate(criticalRate), neverFails(neverFails), preEffect(preEffect), postEffect(postEffect), failEffect(fails)
        {

        }

        Attack::Attack(AttackData const& data) 
            : nameKey(data.nameKey), name(data.nameKey), power(data.power), priority(data.priority), accuracy(data.accuracy), type(data.type), special(data.special), status(data.status), pp(data.ppMax)
            , ppMax(data.ppMax), criticalRate(data.criticalRate), neverFails(data.neverFails), preEffect(data.preEffect), postEffect(data.postEffect), failEffect(data.ifFails)
        {

        }

        Attack::~Attack() {
            delete(this->preEffect);
            delete(this->postEffect);
            delete(this->failEffect);
        }

        Attack* Attack::NewAtk(std::string name) {
            try {
                return new Attack(attackList.at(name));//Work with AssetManager for JSON Loading
            }
            catch (const std::out_of_range&) {
                std::cout << ("Attack " + name + " not found.") << std::endl;
                return nullptr;
            }
        }

        /* Return 1 : Inform to do the same attack at the next turn.
        * Return 2 : End the attack
        * Return -2 : Inform that the attack failed
        * Return -1 : Inform that the attack was ineffective against the target
        * In effectAfter : Return any number except 1 act like return 2.
        * If 1 is returned, it will do the same attack at the next turn.
        * TODO : Create defines to make this more clear
        */
        int Attack::AttackDaemon(Daemon& atk, Daemon& def, std::queue<TurnAction>& turnQueue, bool attacker) {
            pp--;
            //turnQueue.push(createTurnDialogAction(Utils::OpString("battle.dialog.attack", { atk.GetNicknamePtr(), &name })));
            //Attack fail
            if ((Utils::RandU(100)) > (accuracy * (atk.GetStatACC() / def.GetStatEVA())) && neverFails == false) {
                //TurnAction failAction;
                //turnQueue.push(createTurnDialogAction({ Utils::OpString("battle.dialog.fail", {atk.getNicknamePtr()}) }));
                if (failEffect != nullptr) 
                {
                    failEffect->Apply(*this, atk, def, turnQueue);
                }
                return -2;
            }

            int effectBf = preEffect ? preEffect->Apply(*this, atk, def, turnQueue) : 0;
            if (effectBf == 1 || effectBf == 2) { //If special returns 1 or 2, the attack ends.
                return effectBf;
            }
            //If type unefficiency
            if (ArrayTypes::CalcEffectiveness(type, def.GetType1(), def.GetType2()) == 0 && (neverFails == false || status == false)) {
                //turnQueue.push(createTurnDialogAction({ Utils::OpString("battle.effectiveness.none", {atk.getNicknamePtr()}) }));
                if (failEffect != nullptr) 
                {
                    failEffect->Apply(*this, atk, def, turnQueue);
                }
                return -1;
            }


            //Animation time
            //for (TurnActionType tat : animationOrder) {
            //    TurnAction ta;
            //    ta.type = tat;
            //    turnQueue.push(ta);
            //}

            if (!status)//Check if it isn't a status attack to calculate the hp lost
            { 
                hpLost = (((atk.GetLevel() * 0.4 + 2) * (special ? atk.GetStatATKSPE() * atk.GetOtherAtkSpe() : atk.GetStatATK() * atk.GetOtherAtk()) * power) / ((special ? def.GetStatDEFSPE() * def.GetOtherDefSpe() : def.GetStatDEF() * def.GetOtherDef()) * 50) + 2);
                if (type == atk.GetType1() || type == atk.GetType2())//STAB
                {
                    hpLost = round(hpLost * 1.5);
                }
                float effectiveness = (ArrayTypes::CalcEffectiveness(type, def.GetType1(), def.GetType2()));

                //if(effectiveness)//Set effectiveness dialogs here
                hpLost = round(hpLost * effectiveness * def.GetOtherMisc());
                if (Utils::RandU(criticalRate) == 1) {
                    hpLost = round(hpLost * 1.5);
                }

                //Range is 100 to 85 + 1 because not 100 is not inclusive 
                float damageRoll = (Utils::RandU(100.0f - 85.0f + 1.0f) + 85.0f) / 100.0f;
                hpLost = round(hpLost * damageRoll);

                def.Attacked(hpLost);

                //TurnAction loosingHp;
                //NewTurnAction(&loosingHp);
                //loosingHp.type = attacker ? TurnActionType::DEF_UPDATE_HBAR : TurnActionType::ATK_UPDATE_HBAR;
                //loosingHp.hpLost = hpLost;
                //turnQueue.push(loosingHp);

                //if (effectiveness == 0.25)
                //    turnQueue.push(createTurnDialogAction({ Utils::OpString("battle.effectiveness.almostnone") }));
                //else if (effectiveness == 0.5)
                //    turnQueue.push(createTurnDialogAction({ Utils::OpString("battle.effectiveness.notvery") }));
                //else if (effectiveness == 2)
                //    turnQueue.push(createTurnDialogAction({ Utils::OpString("battle.effectiveness.very") }));
                //else if (effectiveness == 4)
                //    turnQueue.push(createTurnDialogAction({ Utils::OpString("battle.effectiveness.super") }));
            }
            return postEffect ? postEffect->Apply(*this, atk, def, turnQueue) : 0;
        }
	}
}
