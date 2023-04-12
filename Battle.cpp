#include "Battle.h"
#include "SpriteComponent.h"
#include "TextRendererComponent.h"

namespace Daemon
{
	namespace Model
	{
        Battle::Battle(Daemon* atk, Daemon* def)
        {
            this->atk = atk;
            this->def = def;
            oldStats[0][0] = atk->GetStatATK();
            oldStats[0][1] = atk->GetStatDEF();
            oldStats[0][2] = atk->GetStatATKSPE();
            oldStats[0][3] = atk->GetStatDEFSPE();
            oldStats[0][4] = atk->GetStatSPE();

            oldStats[1][0] = def->GetStatATK();
            oldStats[1][1] = def->GetStatDEF();
            oldStats[1][2] = def->GetStatATKSPE();
            oldStats[1][3] = def->GetStatDEFSPE();
            oldStats[1][4] = def->GetStatSPE();

            oldTypes[0][0] = atk->GetType1();
            oldTypes[0][1] = atk->GetType2();
            oldTypes[1][0] = def->GetType1();
            oldTypes[1][1] = def->GetType2();

            const Model::Species& atkSpecies = atk->GetSpecies();
            const Model::Species& defSpecies = def->GetSpecies();

            oldSpecies[0] = &atkSpecies;
            oldSpecies[1] = &defSpecies;

            oldAttacks[0] = atk->GetAttacks();
            oldAttacks[1] = def->GetAttacks();

            atk->SetStat(Model::Stats::EVA, 100);
            atk->SetStat(Model::Stats::ACC, 100);
            def->SetStat(Model::Stats::EVA, 100);
            def->SetStat(Model::Stats::ACC, 100);
            //Clear the turns
            NewTurnData(&atkTurn);
            NewTurnData(&defTurn);

            //Register the opmons' addresses in the Turns
            atkTurn.daemon = atk;
            defTurn.daemon = def;
        }

        void Battle::InitalizeUI(std::vector<std::any> ui)
        {
            this->ui["PlayerPokemonPlatform"] = std::any_cast<Entity*>(ui[0]);
            this->ui["PlayerPokemon"] = std::any_cast<Entity*>(ui[1]);
            this->ui["EnemyPokemonPlatform"] = std::any_cast<Entity*>(ui[2]);
            this->ui["EnemyPokemon"] = std::any_cast<Entity*>(ui[3]);

            this->ui["PlayerStatsName"] = std::any_cast<Entity*>(ui[4]);
            this->ui["PlayerStatsLevel"] = std::any_cast<Entity*>(ui[5]);
            this->ui["PlayerStatsHealth"] = std::any_cast<Entity*>(ui[6]);
            this->ui["PlayerStatsHealthText"] = std::any_cast<Entity*>(ui[7]);

            this->ui["EnemyStatsName"] = std::any_cast<Entity*>(ui[8]);
            this->ui["EnemyStatsLevel"] = std::any_cast<Entity*>(ui[9]);
            this->ui["EnemyStatsHealth"] = std::any_cast<Entity*>(ui[10]);
            this->ui["EnemyStatsHealthText"] = std::any_cast<Entity*>(ui[11]);

            this->ui["PlayerTeam0"] = std::any_cast<Entity*>(ui[12]);
            this->ui["PlayerTeam1"] = std::any_cast<Entity*>(ui[13]);
            this->ui["PlayerTeam2"] = std::any_cast<Entity*>(ui[14]);
            this->ui["PlayerTeam3"] = std::any_cast<Entity*>(ui[15]);
            this->ui["PlayerTeam4"] = std::any_cast<Entity*>(ui[16]);
            this->ui["PlayerTeam5"] = std::any_cast<Entity*>(ui[17]);

            this->ui["EnemyTeam0"] = std::any_cast<Entity*>(ui[18]);
            this->ui["EnemyTeam1"] = std::any_cast<Entity*>(ui[19]);
            this->ui["EnemyTeam2"] = std::any_cast<Entity*>(ui[20]);
            this->ui["EnemyTeam3"] = std::any_cast<Entity*>(ui[21]);
            this->ui["EnemyTeam4"] = std::any_cast<Entity*>(ui[22]);
            this->ui["EnemyTeam5"] = std::any_cast<Entity*>(ui[23]);

            this->ui["MoveButton0"] = std::any_cast<Entity*>(ui[24]);
            this->ui["MoveButtonText0"] = std::any_cast<Entity*>(ui[25]);
            this->ui["MoveButtonTextPP0"] = std::any_cast<Entity*>(ui[26]);

            this->ui["MoveButton1"] = std::any_cast<Entity*>(ui[27]);
            this->ui["MoveButtonText1"] = std::any_cast<Entity*>(ui[28]);
            this->ui["MoveButtonTextPP1"] = std::any_cast<Entity*>(ui[29]);

            this->ui["MoveButton2"] = std::any_cast<Entity*>(ui[30]);
            this->ui["MoveButtonText2"] = std::any_cast<Entity*>(ui[31]);
            this->ui["MoveButtonTextPP2"] = std::any_cast<Entity*>(ui[32]);

            this->ui["MoveButton3"] = std::any_cast<Entity*>(ui[33]);
            this->ui["MoveButtonText3"] = std::any_cast<Entity*>(ui[34]);
            this->ui["MoveButtonTextPP3"] = std::any_cast<Entity*>(ui[35]);

            this->ui["BattleTextBox"] = std::any_cast<Entity*>(ui[36]);             
        }

        void Battle::Turn()
        {           
            //These variables are used to check if the turn of one of the OpMons' is over.
            bool atkDone = false;
            bool defDone = false;

            AITurn();

            /*if (!actionsQueue.empty()) {
                handleError("Error : Actions Queue not empty but beginning a new turn anyway. Undefined behavior my result, because I won't fix that for you. And it could be funny to see.");
            }*/

            //Item use or switching always comes before the attack. It is calculated before everything else.
            if (atkTurn.type != TurnType::ATTACK) {
                //Actions
                atkDone = true;
            }
            if (defTurn.type != TurnType::ATTACK) {
                //Actions
                defDone = true;
            }
            //If the two of them attack, then the priority must be calculated. Else, the only attacking OpMon will attack, obviously.
            if (defTurn.type == TurnType::ATTACK && atkTurn.type == TurnType::ATTACK) 
            {
                
                atkFirst = (atkTurn.attackUsed->GetPriority() == defTurn.attackUsed->GetPriority()) ? (atk->GetStatSPE() * atk->GetOtherSpe() > def->GetStatSPE() * def->GetOtherSpe()) : (atkTurn.attackUsed->GetPriority() > defTurn.attackUsed->GetPriority());
            }
            else 
            {
                atkFirst = !atkDone;
            }

            if (!atkDone || !defDone) 
            {
                if (atkFirst) 
                {
                    if (!atkDone && CanAttack(atk, &atkTurn)) 
                    {
                        atkTurn.attackUsed->attack(*atk, *def, actionsQueue, true);
                    }
                    //actionsQueue.push(next);
                    if (!defDone && CanAttack(def, &defTurn) && !CheckBattleEnded()) 
                    {
                        defTurn.attackUsed->attack(*def, *atk, actionsQueue, false);
                    }

                    CheckBattleEnded();

                }
                else
                {
                    if (!defDone && CanAttack(def, &defTurn))
                    {
                        defTurn.attackUsed->attack(*def, *atk, actionsQueue, false);
                    }
                    //actionsQueue.push(next);
                    if (!atkDone && CanAttack(atk, &atkTurn) && !CheckBattleEnded())
                    {
                        atkTurn.attackUsed->attack(*atk, *def, actionsQueue, true);
                    }
                    CheckBattleEnded();
                }
            }

            //Resets all Other Stats that are just for the turn;
            atk->ResetAllOtherStats();
            def->ResetAllOtherStats();
        }

        bool Battle::CanAttack(Daemon* daemon, TurnData* daemonTurn)
		{
            bool canAttack = true;
            //std::vector<std::string*> opName(1);
            //opName[0] = &daemon->GetNickname();
            //Checks if frozen
            if (daemon->GetStatus() == Model::Status::FROZEN) 
            {
                //The OpMon have one chance out of 5 to be able to move again.
                if (Utils::randU(5) == 1) 
                {
                    //actionsQueue.push(createTurnDialogAction({ Utils::OpString("battle.status.frozen.out", opName) }));
                    daemon->SetStatus(Model::Status::NOTHING);
                }
                else
                {
                    //actionsQueue.push(createTurnDialogAction({ Utils::OpString("battle.status.frozen.attack", opName) }));
                    canAttack = false;
                }
                //Checks if sleeping
            }
            else if (daemon->GetStatus() == Model::Status::SLEEPING)
            {
                //Checks the sleep counter.
                if (daemon->GetSleepingCD() <= 0)
                {
                    //actionsQueue.push(createTurnDialogAction({ Utils::OpString("battle.status.sleep.out", opName) }));
                    daemon->SetStatus(Status::NOTHING);
                }
                else 
                {
                    //actionsQueue.push(createTurnDialogAction({ Utils::OpString("battle.status.sleep.attack", opName) }));
                    canAttack = false;
                    daemon->PassCD(true);
                }
                //Checks if paralysed
            }
            else if (daemon->GetStatus() == Model::Status::PARALYSED) 
            {
                //The opmon have one chance out of three to can't attack when paralysed
                if (Utils::randU(3) == 1) 
                {
                    //actionsQueue.push(createTurnDialogAction(Utils::OpString("battle.status.paralysed.attack.fail", opName)));
                    canAttack = false;
                }
                else 
                {
                    //actionsQueue.push(createTurnDialogAction(Utils::OpString("battle.status.paralysed.attack.success", opName)));
                }
            }
            //Checks if confused
            if (daemon->confused) 
            {
                //Checks the confused counter
                if (daemon->GetConfusedCD() <= 0) 
                {
                    daemon->confused = false;
                    //actionsQueue.push(createTurnDialogAction({ Utils::OpString("battle.status.confused.out", opName) }));
                }
                else 
                {
                    daemon->PassCD(false);
                    //The OpMon have one chance out of two of failing their attack.
                    if (Utils::randU(2) == 1) 
                    {
                        //actionsQueue.push(createTurnDialogAction({ Utils::OpString("battle.status.confused.attack.fail", opName) }));
                        daemon->Attacked(daemon->GetStatHP() / 8);
                    }
                    else 
                    {
                       //actionsQueue.push(createTurnDialogAction(Utils::OpString("battle.status.confused.attack.success", opName)));
                    }
                }
            }

            //Checks if afraid
            if (daemon->afraid) 
            {
                //actionsQueue.push(createTurnDialogAction({ Utils::OpString("battle.status.afraid", opName) }));
                daemon->afraid = false;
                canAttack = false;
            }
            //delete(opName[0]);
            return canAttack;
		}

        TurnData* Battle::AITurn()
        {
            defTurn.attackUsed = def->GetAttacks()[0];
            defTurn.type = TurnType::ATTACK;
            return &defTurn;
        }

        bool Battle::CheckBattleEnded()
        {
            if (def->GetHP() <= 0) 
            {
                //if (trainer != nullptr) 
                //{
                //    trainer->defeat();
                //}

                //TurnAction victory;
                //victory.type = TurnActionType::VICTORY;
                //actionsQueue.push(victory);

                std::cout << "Battle Ended" << std::endl;
                return true;
            }
            else if (atk->GetHP() <= 0) 
            {
                //if (trainer != nullptr) { //TODO : Only for the test battle, should be removed at some point.
                //    trainer->defeat();
                //}

                //TurnAction defeat;
                //defeat.type = TurnActionType::DEFEAT;
                //actionsQueue.push(defeat);

                std::cout << "Battle Ended" << std::endl;
                return true;
            }
            return false;
        }

        void Battle::UpdateBattleUI()
        {
            ui["PlayerPokemon"]->getComponent<SpriteComponent>().ClearSetTexture(atk->GetSpecies().getName());
            ui["EnemyPokemon"]->getComponent<SpriteComponent>().ClearSetTexture(def->GetSpecies().getName());

            ui["PlayerStatsName"]->getComponent<TextRendererComponent>().SetText(atk->GetNickname());
            ui["PlayerStatsLevel"]->getComponent<TextRendererComponent>().SetText("lvl: " + std::to_string(atk->GetLevel()));
            ui["PlayerStatsHealth"]->getComponent<RectComponent>().scale.x = 504.0f * atk->GetHP() / atk->GetStatHP();
            ui["PlayerStatsHealthText"]->getComponent<TextRendererComponent>().SetText(std::to_string(atk->GetHP()) + "/" + std::to_string(atk->GetStatHP()));

            ui["EnemyStatsName"]->getComponent<TextRendererComponent>().SetText(atk->GetNickname());
            ui["EnemyStatsLevel"]->getComponent<TextRendererComponent>().SetText("lvl: " + std::to_string(atk->GetLevel()));
            ui["EnemyStatsHealth"]->getComponent<RectComponent>().scale.x = 504.0f * atk->GetHP() / atk->GetStatHP();
            ui["EnemyStatsHealthText"]->getComponent<TextRendererComponent>().SetText(std::to_string(atk->GetHP()) + "/" + std::to_string(atk->GetStatHP()));
            /*
            this->ui["PlayerTeam0"] = std::any_cast<Entity*>(ui[12]);
            this->ui["PlayerTeam1"] = std::any_cast<Entity*>(ui[13]);
            this->ui["PlayerTeam2"] = std::any_cast<Entity*>(ui[14]);
            this->ui["PlayerTeam3"] = std::any_cast<Entity*>(ui[15]);
            this->ui["PlayerTeam4"] = std::any_cast<Entity*>(ui[16]);
            this->ui["PlayerTeam5"] = std::any_cast<Entity*>(ui[17]);

            this->ui["EnemyTeam0"] = std::any_cast<Entity*>(ui[18]);
            this->ui["EnemyTeam1"] = std::any_cast<Entity*>(ui[19]);
            this->ui["EnemyTeam2"] = std::any_cast<Entity*>(ui[20]);
            this->ui["EnemyTeam3"] = std::any_cast<Entity*>(ui[21]);
            this->ui["EnemyTeam4"] = std::any_cast<Entity*>(ui[22]);
            this->ui["EnemyTeam5"] = std::any_cast<Entity*>(ui[23]);
            */

            for (int i = 0; i < 4; i++) 
            {
                bool active = atk->GetAttacks().size() > i;
                ui["MoveButton" + std::to_string(i)]->SetActiveStatus(active);
                if (active)
                {
                    ui["MoveButtonText" + std::to_string(i)]->getComponent<TextRendererComponent>().SetText(atk->GetAttacks()[i]->GetName());
                    ui["MoveButtonTextPP" + std::to_string(i)]->getComponent<TextRendererComponent>().SetText(std::to_string(atk->GetAttacks()[i]->GetPP()) + "/" + std::to_string(atk->GetAttacks()[i]->GetPPMax()));
                }
            }

            /*
            this->ui["BattleTextBox"] = std::any_cast<Entity*>(ui[36]);*/
        }
	}
}
