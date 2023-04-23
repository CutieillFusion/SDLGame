#include "Battle.h"
#include "SpriteComponent.h"
#include "TextRendererComponent.h"

namespace Daemon
{
	namespace Model
	{
        static Battle* instance;
        static std::map<std::string, Entity*> ui;

        Battle::Battle(DaeTeam* playerTeam, DaeTeam* trainerTeam, int atkIndex, int defIndex)
        {
            instance = this;

            this->playerTeam = playerTeam;
            this->trainerTeam = trainerTeam;

            this->atk = playerTeam->operator[](atkIndex);
            this->def = trainerTeam->operator[](defIndex);

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

            ui["WorldParent"]->SetActiveStatus(false);
            ui["BattleUIParent"]->SetActiveStatus(true);

            UpdateBattleUI();
        }

        void Battle::InitalizeUI(std::map<std::string, Entity*> _ui)
        {
            ui = _ui;                
        }

        //Move Order
        //  Player Run
        //  Priority Item or Enemy Run(Wild Daemon)
        //  Item or Change
        //  Attacks
        void Battle::Turn()
        {           
            //These variables are used to check if the turn of one of the OpMons' is over.
            bool atkDone = false;
            bool defDone = false;

            AITurn();

            /*if (!actionsQueue.empty()) {
                handleError("Error : Actions Queue not empty but beginning a new turn anyway. Undefined behavior my result, because I won't fix that for you. And it could be funny to see."];
            }*/

            //Item use or switching always comes before the attack. It is calculated before everything else.
            if (atkTurn.type != TurnType::ATTACK) 
            {
                //Actions
                if (atkTurn.type == TurnType::CHANGE) 
                {
                    atk = atkTurn.daemon;
                }
                atkDone = true;
            }
            if (defTurn.type != TurnType::ATTACK) 
            {
                //Actions
                if (defTurn.type == TurnType::CHANGE)
                {
                    def = defTurn.daemon;
                }

                defDone = true;
            }

            //If the two of them attack, then the priority must be calculated. Else, the only attacking Daemon will attack, obviously.
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
                    if (!defDone && CanAttack(def, &defTurn) && !CheckTurnEnded()) 
                    {
                        defTurn.attackUsed->attack(*def, *atk, actionsQueue, false);
                    }
                }
                else
                {
                    if (!defDone && CanAttack(def, &defTurn))
                    {
                        defTurn.attackUsed->attack(*def, *atk, actionsQueue, false);
                    }
                    //actionsQueue.push(next);
                    if (!atkDone && CanAttack(atk, &atkTurn) && !CheckTurnEnded())
                    {
                        atkTurn.attackUsed->attack(*atk, *def, actionsQueue, true);
                    }
                }

            }
            UpdateBattleUI();

            //Resets all Other Stats that are just for the turn;
            NewTurnData(&atkTurn);
            NewTurnData(&defTurn);

            atk->ResetAllOtherStats();
            def->ResetAllOtherStats();

            if (CheckBattleEnded())
            {
                EndBattle();
            }
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
            defTurn.daemon = def;
            defTurn.type = TurnType::ATTACK;
            return &defTurn;
        }

        bool Battle::PlayerTurn(int moveIndex)
        {
            if (moveIndex < atk->GetAttacks().size() && !atk->IsDead()) 
            {
                if (atk->GetAttacks()[moveIndex]->GetPP() > 0)
                {
                    atkTurn.daemon = atk;
                    atkTurn.attackUsed = atk->GetAttacks()[moveIndex];
                    atkTurn.type = TurnType::ATTACK;
                    return true;
                }
            }
            return false;
        }

        bool Battle::SwapTurn(int daemonIndex)
        {
            if (daemonIndex >= playerTeam->GetSize())
            {
                return false;
            }

            auto newDaemon = playerTeam->operator[](daemonIndex);
            if (newDaemon != atk && !newDaemon->IsDead()) 
            {
                if (!atk->IsDead()) 
                {
                    atkTurn.daemon = newDaemon;
                    atkTurn.type = TurnType::CHANGE;
                    return true;
                }
                else 
                {
                    atk = newDaemon;
                    return false;
                }
            }

            return false;
        }

        bool Battle::ItemTurn(Item* item)
        {
            atkTurn.itemUsed = item;
            atkTurn.type = TurnType::ITEM;
            return true;
        }

        bool Battle::RunTurn()
        {
            atkTurn.type = TurnType::RUN;
            return true;
        }

        bool Battle::CheckTurnEnded()
        {
            if (def->IsDead()) 
            {
                //if (trainer != nullptr) 
                //{
                //    trainer->defeat();
                //}

                //TurnAction victory;
                //victory.type = TurnActionType::VICTORY;
                //actionsQueue.push(victory);

                return true;
            }
            else if (atk->IsDead())
            {
                //if (trainer != nullptr) { //TODO : Only for the test battle, should be removed at some point.
                //    trainer->defeat();
                //}

                //TurnAction defeat;
                //defeat.type = TurnActionType::DEFEAT;
                //actionsQueue.push(defeat);

                return true;
            }
            return false;
        }

        bool Battle::CheckBattleEnded()
        {
            if (playerTeam->IsKo()) 
            {
                std::cout << "Player Lost Battle" << std::endl;
                EndBattle();
                return true;
            }
            if (trainerTeam->IsKo()) 
            {
                std::cout << "Player Won Battle" << std::endl;
                EndBattle();
                return true;
            }

            return false;
        }

        void Battle::EndBattle()
        {
            ui["BattleUIParent"]->SetActiveStatus(false);
            ui["WorldParent"]->SetActiveStatus(true);
            //delete this;
        }

        std::vector<std::string> pokeballIds =
        {
            "Pokeball_Team", "Pokeball_Team_Highlighted", "Pokeball_Team_Activated"
        };

        Battle::~Battle()
        {
            instance = nullptr;
        }

        void Battle::UpdateBattleUI()
        {
            ui["PlayerPokemon"]->getComponent<SpriteComponent>().ClearSetTexture(atk->GetSpecies().getName());
            ui["EnemyPokemon"]->getComponent<SpriteComponent>().ClearSetTexture(def->GetSpecies().getName());

            ui["PlayerStatsName"]->getComponent<TextRendererComponent>().SetText(atk->GetNicknamePtr());//*
            ui["PlayerStatsLevel"]->getComponent<TextRendererComponent>().SetText("lvl: " + std::to_string(atk->GetLevel()));//*
            ui["PlayerStatsHealth"]->getComponent<RectComponent>().scale.x = 504.0f * atk->GetHP() / atk->GetStatHP();
            ui["PlayerStatsHealthText"]->getComponent<TextRendererComponent>().SetText(std::to_string(atk->GetHP()) + "/" + std::to_string(atk->GetStatHP()));//*
            
            ui["EnemyStatsName"]->getComponent<TextRendererComponent>().SetText(def->GetNicknamePtr());//*
            ui["EnemyStatsLevel"]->getComponent<TextRendererComponent>().SetText("lvl: " + std::to_string(def->GetLevel()));//*
            ui["EnemyStatsHealth"]->getComponent<RectComponent>().scale.x = 504.0f * def->GetHP() / def->GetStatHP();
            ui["EnemyStatsHealthText"]->getComponent<TextRendererComponent>().SetText(std::to_string(def->GetHP()) + "/" + std::to_string(def->GetStatHP()));//*

            for (int i = 0; i < 6; i++)
            {
                if (playerTeam->GetSize() > i) 
                {
                    std::string name = playerTeam->GetDae(i)->GetSpecies().getName();
                    ui["PlayerTeam" + std::to_string(i)]->getComponent<SpriteComponent>().ClearSetTexture({ name, name, name });
                }
                else 
                {
                    ui["PlayerTeam" + std::to_string(i)]->getComponent<SpriteComponent>().ClearSetTexture({ "Pokeball_Team", "Pokeball_Team_Highlighted", "Pokeball_Team_Activated" });
                }
            }

            for (int i = 0; i < 6; i++)
            {
                if (trainerTeam->GetSize() > i)
                {
                    std::string name = trainerTeam->GetDae(i)->GetSpecies().getName();
                    ui["EnemyTeam" + std::to_string(i)]->getComponent<SpriteComponent>().ClearSetTexture({ name, name, name });
                }
                else
                {
                    ui["EnemyTeam" + std::to_string(i)]->getComponent<SpriteComponent>().ClearSetTexture(pokeballIds);

                }
            }

            for (int i = 0; i < 4; i++) 
            {
                bool active = atk->GetAttacks().size() > i && ui["BattleUIParent"]->isActive();
                ui["MoveButton" + std::to_string(i)]->SetActiveStatus(active);
                if (active)
                {
                    ui["MoveButtonText" + std::to_string(i)]->getComponent<TextRendererComponent>().SetText(atk->GetAttacks()[i]->GetName());
                    ui["MoveButtonTextPP" + std::to_string(i)]->getComponent<TextRendererComponent>().SetText(std::to_string(atk->GetAttacks()[i]->GetPP()) + "/" + std::to_string(atk->GetAttacks()[i]->GetPPMax()));
                }
            }
        }

        namespace BattleWrapper 
        {
            void UseMove(std::vector<std::any> data)
            {
                auto index = std::any_cast<int>(data[0]);

                if (instance != nullptr) 
                {
                    if (instance->PlayerTurn(index)) 
                    {
                        instance->Turn();
                    }
                }
            }

            void SwapDaemon(std::vector<std::any> data)
            {
                auto index = std::any_cast<int>(data[0]);

                if (instance != nullptr)
                {
                    if (instance->SwapTurn(index))
                    {
                        instance->Turn();
                    }
                }
            }
        }
	}
}
