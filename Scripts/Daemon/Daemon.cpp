#include "Daemon.h"
#include "Evolution.h"
#include "Evolutions.h"
#include "Attacks.h"
#include "Item.h"
#include <fstream>
#include <sstream>

namespace Daemon 
{
	namespace Model 
	{
		Daemon::~Daemon() 
		{
			for (int i = 0; i < 4; i++) {
				delete(attacks[i]);
			}
		}

		Daemon::Daemon(const std::string& nickname, const Species* species, int level, const std::vector<Attack*>& attacks, Nature nature)
			: nickname((nickname.empty()) ? species->GetName() : nickname), species(species), level(level), attacks(attacks), nature(nature)
		{
			atkIV = Utils::RandU(32);
			defIV = Utils::RandU(32);
			atkSpeIV = Utils::RandU(32);
			defSpeIV = Utils::RandU(32);
			speIV = Utils::RandU(32);
			hpIV = Utils::RandU(32);
			CalcStats();

			captureRate = species->GetCaptureRate();
			HP = statHP;
			type1 = species->GetType1();
			type2 = species->GetType2();

			auto curve = this->species->GetCurve();
			this->toNextLevel = curve->GetNeededExp(this->level + 1);
			this->exp = curve->GetNeededExp(this->level);

			held = nullptr;
			statLove = 100;
			statACC = 100;
			statEVA = 100;
			initialized = false;
		}

		void Daemon::PassCD(bool sleep)
		{
			if (confusedCD > 0 && !sleep) 
			{
				confusedCD--;
			}
			else if (sleepingCD > 0 && sleep) 
			{
				sleepingCD--;
			}
		}

		bool Daemon::Captured(Item const& OpBox)
		{
			//Capture Rate
			int a = round((((3 * statHP - 2 * HP) * captureRate * (status == Status::PARALYSED || status == Status::POISONED || status == Status::BURNING ? 1.5 : (status == Status::FROZEN || status == Status::SLEEPING ? 2 : 1))) / (3 * statHP)));
			//Shake Check
			int b = round((pow(2, 16) - 1) * pow(a / (pow(2, 8) - 1), 0.25));
			int c[] = { Utils::RandU(65535), Utils::RandU(65535), Utils::RandU(65535), Utils::RandU(65535) };
			
			int nbreOk = 0;
			for (int i = 0; i < 4; i++) 
			{
				if (c[i] <= b) 
				{
					nbreOk++;
				}
			}

			if (nbreOk == 0)//nbreOk represents the number of times the OpBall wriggles
			{
				return false;
			}
			else 
			{
				//there is multiple "if" here because the printed dialog is different. Since this method is unused for now, this isn't very problematic to leave it as it is.
				if (nbreOk == 1)
				{
					return false;
				}
				if (nbreOk == 2) 
				{
					return false;
				}
				if (nbreOk == 3) 
				{
					return false;
				}
				if (nbreOk != 4)//An DaemonBall can't wriggle four times
				{ 
					//handleError("InternalError : OpMon.cpp : nbreOk != 4", true);
				}
				return true;
			}
		}

		void Daemon::SetStat(Stats stat, int newStat)
		{
			switch (stat) 
			{
			case Stats::ATK:
				statATK = newStat;
				break;
			case Stats::DEF:
				statDEF = newStat;
				break;
			case Stats::ATKSPE:
				statATKSPE = newStat;
				break;
			case Stats::DEFSPE:
				statDEFSPE = newStat;
				break;
			case Stats::SPE:
				statSPE = newStat;
				break;
			case Stats::HP:
				statHP = newStat;
				break;
			case Stats::EVA:
				statEVA = newStat;
				break;
			case Stats::ACC:
				statACC = newStat;
				break;
			case Stats::NOTHING:
				//Utils::Log::oplog("[WARNING] - Incorrect value in a switch (OpMon::setStat). Expected a stat, got Stats::NOTHING.");
				break;
			}
		}
		void Daemon::LevelUp()
		{
			level++;

			//Calcs the exp needed for next level
			auto curve = this->species->GetCurve();
			this->toNextLevel = curve->GetNeededExp(this->level + 1);
			this->exp = curve->GetNeededExp(this->level);

			CalcStats();

			//Check if Daemon Evolves
			if (species->GetEvolType()->CheckEvolve(*this)) 
			{
				Evolve();
			}
		}

		int Daemon::Win(Daemon const& defeated)
		{
			GetEvs(defeated);
			int expGain = ((defeated.species->GetExp() * defeated.level) / (float)this->level) * expBoost;
			exp += expGain;

			//Handles the Level Won. The loops allows to increase the level depending on the exp gain
			while (exp >= toNextLevel && level <= 100) 
			{
				if (exp < toNextLevel) 
				{
					break;
				}
				LevelUp();
			}
			CalcStats();

			return expGain;
		}

		void Daemon::Heal(int HP)
		{
			this->HP = (HP + this->HP >= statHP) ? statHP : HP + this->HP;
			//this->HP = std::min(statHP, HP + this->HP);
		}

		void Daemon::GetEvs(Daemon const& defeated)
		{
			//If the total of the Evs is over 510 ends because ezs max out at 510
			if (!((atkEV + defEV + hpEV + atkSpeEV + defSpeEV + speEV) > MAX_EVS))
			{
				//Rewrite in the future???
				std::vector<Stats> statsDefeated;
				for (unsigned int i = 0; i < defeated.species->GetEv().size(); i++)
				{
					statsDefeated.push_back(defeated.species->GetEv()[i]);
				}

				for (unsigned int i = 0; i < statsDefeated.size(); i++) 
				{
					switch (statsDefeated[i]) 
					{
					case Stats::ATK:
						if (atkEV < MAX_EV) 
						{
							atkEV++;
						}
						break;
					case Stats::ATKSPE:
						if (atkSpeEV < MAX_EV)
						{
							atkSpeEV++;
						}
						break;
					case Stats::DEF:
						if (defEV < MAX_EV)
						{
							atkEV++;
						}
						break;
					case Stats::DEFSPE:
						if (defSpeEV < MAX_EV)
						{
							defSpeEV++;
						}
						break;
					case Stats::HP:
						if (hpEV < MAX_EV)
						{
							hpEV++;
						}
						break;
					case Stats::NOTHING:
						break;
					case Stats::SPE:
						if (speEV < MAX_EV)
						{
							speEV++;
						}
						break;
					default:
						break;
					}
				}
			}
		}

		void Daemon::SetType1(Type type)
		{
			this->type1 = type;
		}

		void Daemon::SetType2(Type type)
		{
			this->type2 = type;
		}

		void Daemon::CalcStats() {
			//Big annoying formulas
			statATK = round(
				((((2 * species->GetBaseAtk() + atkIV + (atkEV / 4)) * level) / 100) + 5) * 
				((natures[(int)nature].positive == Stats::ATK) ? 1.1 : ((natures[(int)nature].negative == Stats::ATK) ? 0.9 : 1)));
			statDEF = round(
				((((2 * species->GetBaseDef() + defIV + (defEV / 4)) * level) / 100) + 5) * 
				((natures[(int)nature].positive == Stats::DEF) ? 1.1 : ((natures[(int)nature].negative == Stats::DEF) ? 0.9 : 1)));
			statATKSPE =
				round(((((2 * species->GetBaseAtkSpe() + atkSpeIV + (atkSpeEV / 4)) * level) / 100) + 5) * 
					((natures[(int)nature].positive == Stats::ATKSPE) ? 1.1 : ((natures[(int)nature].negative == Stats::ATKSPE) ? 0.9 : 1)));
			statDEFSPE =
				round(((((2 * species->GetBaseDefSpe() + defSpeIV + (defSpeEV / 4)) * level) / 100) + 5) * 
					((natures[(int)nature].positive == Stats::DEFSPE) ? 1.1 : ((natures[(int)nature].negative == Stats::DEFSPE) ? 0.9 : 1)));
			statSPE = round(
				((((2 * species->GetBaseSpe() + speIV + (speEV / 4)) * level) / 100) + 5) * 
				((natures[(int)nature].positive == Stats::SPE) ? 1.1 : ((natures[(int)nature].negative == Stats::SPE) ? 0.9 : 1)));
			
			statHP = round(((2 * species->GetBaseHP() + hpIV + (hpEV / 4)) * level) / 100) + level + 10;
		}

		Item* Daemon::Hold(Item* item)
		{
			//Swaps
			Item* old = held;
			held = item;
			//Returns Old Item
			return old;
		}

		void Daemon::Traded()
		{
			//This might be changed in the future
			expBoost = 1.5f;
		}

		void Daemon::Evolve()
		{
			species = species->GetEvolution();
		}

		//Makes a struct in the future?
		void Daemon::SetStats(int stats[], Attack* attacks[], const Species& species, Type types[])
		{
			statATK = stats[0];
			statDEF = stats[1];
			statATKSPE = stats[2];
			statDEFSPE = stats[3];
			statSPE = stats[4];
			statACC = 100;
			statEVA = 100;
			type1 = types[0];
			type2 = types[1];
			this->species = &species;
			this->attacks[0] = attacks[0];
			this->attacks[1] = attacks[1];
			this->attacks[2] = attacks[2];
			this->attacks[3] = attacks[3];
			type1 = types[0];
			type2 = types[1];
		}

		void Daemon::Attacked(int hpLost)
		{
			HP -= hpLost;
			
			//Clamp HP between 0 to HP
			HP = (HP < 0) ? 0 : HP;
		}

		#pragma region CHANGE_STAT

		int Daemon::ChangeATK(int power)
		{
			if (power < 0) 
			{
				for (int i = 0; i > power; i--) 
				{
					switch (atkChange) 
					{
					case -6:
						return -7;
					case -5:
						statATK = round(statATK / 1.16);
						atkChange--;
						break;
					case -4:

						statATK = round(statATK * 0.878788);
						atkChange--;
						break;
					case -3:

						statATK = round(statATK * 0.825);
						atkChange--;
						break;
					case -2:

						statATK = round(statATK / 1.25);
						atkChange--;
						break;
					case -1:

						statATK = round(statATK / 1.34);
						atkChange--;
						break;
					case 0:

						statATK = round(statATK * 0.67);
						atkChange--;
						break;
					case 1:

						statATK = round(statATK / 1.5);
						atkChange--;
						break;
					case 2:

						statATK = round(statATK * 0.75);
						atkChange--;
						break;
					case 3:

						statATK = round(statATK / 1.25);
						atkChange--;
						break;
					case 4:

						statATK = round(statATK / 1.2);
						atkChange--;
						break;
					case 5:

						statATK = round(statATK / 1.1667);
						atkChange--;
						break;
					case 6:

						statATK = round(statATK * 0.875);
						atkChange--;
						break;
					default:
						break;
					}
				}
			}
			else 
			{
				for (int i = 0; i > power; i--) 
				{
					switch (atkChange) 
					{
					case -6:

						statATK = round(statATK * 1.16);
						atkChange++;
						break;
					case -5:

						statATK = round(statATK / 0.878788);
						atkChange++;
						break;
					case -4:

						statATK = round(statATK / 0.825);
						atkChange++;
						break;
					case -3:

						statATK = round(statATK * 1.25);
						atkChange++;
						break;
					case -2:

						statATK = round(statATK * 1.34);
						atkChange++;
						break;
					case -1:

						statATK = round(statATK / 0.67);
						atkChange++;
						break;
					case 0:

						statATK = round(statATK * 1.5);
						atkChange++;
						break;
					case 1:

						statATK = round(statATK / 0.75);
						atkChange++;
						break;
					case 2:

						statATK = round(statATK * 1.25);
						atkChange++;
						break;
					case 3:

						statATK = round(statATK * 1.2);
						atkChange++;
						break;
					case 4:

						statATK = round(statATK * 1.1667);
						atkChange++;
						break;
					case 5:

						statATK = round(statATK / 0.875);
						atkChange++;
						break;
					case 6:
						return 7;
					default:
						break;
					}
				}
			}

			return power;
		}

		int Daemon::ChangeACC(int power)
		{
			if (power < 0) 
			{
				for (int i = 0; i > power; i--) 
				{
					switch (accChange) 
					{
					case -6:
						return -7;
					case -5:

						statACC = round(statACC * 0.888);
						accChange--;
						break;
					case -4:

						statACC = round(statACC / 1.144);
						accChange--;
						break;
					case -3:

						statACC = round(statACC * 0.858);
						accChange--;
						break;
					case -2:

						statACC = round(statACC / 1.2);
						accChange--;
						break;
					case -1:

						statACC = round(statACC / 1.25);
						accChange--;
						break;
					case 0:

						statACC = round(statACC * 0.75);
						accChange--;
						break;
					case 1:

						statACC = round(statACC / 1.333);
						accChange--;
						break;
					case 2:

						statACC = round(statACC * 0.799640);
						accChange--;
						break;
					case 3:

						statACC = round(statACC * 0.8335);
						accChange--;
						break;
					case 4:

						statACC = round(statACC / 1.1665);
						accChange--;
						break;
					case 5:

						statACC = round(statACC * 0.8747657);
						accChange--;
						break;
					case 6:

						statACC = round(statACC * 0.889);
						accChange--;
						break;
					default:
						break;
					}
				}
			}
			else 
			{

				for (int i = 0; i > power; i--) 
				{
					switch (accChange) 
					{
					case -6:

						statACC = round(statACC / 0.888);
						accChange++;
						break;
					case -5:

						statACC = round(statACC * 1.144);
						accChange++;
						break;
					case -4:

						statACC = round(statACC / 0.858);
						accChange++;
						break;
					case -3:

						statACC = round(statACC * 1.2);
						accChange++;
						break;
					case -2:

						statACC = round(statACC * 1.25);
						accChange++;
						break;
					case -1:

						statACC = round(statACC / 0.75);
						accChange++;
						break;
					case 0:

						statACC = round(statACC * 1.333);
						accChange++;
						break;
					case 1:

						statACC = round(statACC / 0.799640);
						accChange++;
						break;
					case 2:

						statACC = round(statACC / 0.8335);
						accChange++;
						break;
					case 3:

						statACC = round(statACC * 1.1665);
						accChange++;
						break;
					case 4:

						statACC = round(statACC / 0.8747657);
						accChange++;
						break;
					case 5:

						statACC = round(statACC / 0.889);
						accChange++;
						break;
					case 6:
						return 7;
					default:
						break;
					}
				}
			}

			return power;
		}

		int Daemon::ChangeEVA(int power)
		{
			return 0; if (power < 0) 
			{
				for (int i = 0; i > power; i--) 
				{
					switch (evaChange) 
					{
					case -6:
						return -7;
					case -5:

						statEVA = round(statEVA / 1.16);
						evaChange--;
						break;
					case -4:

						statEVA = round(statEVA * 0.878788);
						evaChange--;
						break;
					case -3:

						statEVA = round(statEVA * 0.825);
						evaChange--;
						break;
					case -2:

						statEVA = round(statEVA / 1.25);
						evaChange--;
						break;
					case -1:

						statEVA = round(statEVA / 1.34);
						evaChange--;
						break;
					case 0:

						statEVA = round(statEVA * 0.67);
						evaChange--;
						break;
					case 1:

						statEVA = round(statEVA / 1.5);
						evaChange--;
						break;
					case 2:

						statEVA = round(statEVA * 0.75);
						evaChange--;
						break;
					case 3:

						statEVA = round(statEVA / 1.25);
						evaChange--;
						break;
					case 4:

						statEVA = round(statEVA / 1.2);
						evaChange--;
						break;
					case 5:

						statEVA = round(statEVA / 1.1667);
						evaChange--;
						break;
					case 6:

						statEVA = round(statEVA * 0.875);
						evaChange--;
						break;
					default:
						break;
					}
				}
			}
			else 
			{

				for (int i = 0; i > power; i--) 
				{
					switch (evaChange) 
					{
					case -6:

						statEVA = round(statEVA * 1.16);
						evaChange++;
						break;
					case -5:

						statEVA = round(statEVA / 0.878788);
						evaChange++;
						break;
					case -4:

						statEVA = round(statEVA / 0.825);
						evaChange++;
						break;
					case -3:

						statEVA = round(statEVA * 1.25);
						evaChange++;
						break;
					case -2:

						statEVA = round(statEVA * 1.34);
						evaChange++;
						break;
					case -1:

						statEVA = round(statEVA / 0.67);
						evaChange++;
						break;
					case 0:

						statEVA = round(statEVA * 1.5);
						evaChange++;
						break;
					case 1:

						statEVA = round(statEVA / 0.75);
						evaChange++;
						break;
					case 2:

						statEVA = round(statEVA * 1.25);
						evaChange++;
						break;
					case 3:

						statEVA = round(statEVA * 1.2);
						evaChange++;
						break;
					case 4:

						statEVA = round(statEVA * 1.1667);
						evaChange++;
						break;
					case 5:

						statEVA = round(statEVA / 0.875);
						evaChange++;
						break;
					case 6:
						return 7;
					default:
						break;
					}
				}
			}

			return power;
		}

		int Daemon::ChangeDEF(int power)
		{
			if (power < 0) 
			{
				for (int i = 0; i > power; i--) 
				{
					switch (defChange) 
					{
					case -6:
						return -7;
					case -5:

						statDEF = round(statDEF / 1.16);
						defChange--;
						break;
					case -4:

						statDEF = round(statDEF * 0.878788);
						defChange--;
						break;
					case -3:

						statDEF = round(statDEF * 0.825);
						defChange--;
						break;
					case -2:

						statDEF = round(statDEF / 1.25);
						defChange--;
						break;
					case -1:

						statDEF = round(statDEF / 1.34);
						defChange--;
						break;
					case 0:

						statDEF = round(statDEF * 0.67);
						defChange--;
						break;
					case 1:

						statDEF = round(statDEF / 1.5);
						defChange--;
						break;
					case 2:

						statDEF = round(statDEF * 0.75);
						defChange--;
						break;
					case 3:

						statDEF = round(statDEF / 1.25);
						defChange--;
						break;
					case 4:

						statDEF = round(statDEF / 1.2);
						defChange--;
						break;
					case 5:

						statDEF = round(statDEF / 1.1667);
						defChange--;
						break;
					case 6:

						statDEF = round(statDEF * 0.875);
						defChange--;
						break;
					default:
						break;
					}
				}
			}
			else 
			{

				for (int i = 0; i > power; i--) 
				{
					switch (defChange) 
					{
					case -6:

						statDEF = round(statDEF * 1.16);
						defChange++;
						break;
					case -5:

						statDEF = round(statDEF / 0.878788);
						defChange++;
						break;
					case -4:

						statDEF = round(statDEF / 0.825);
						defChange++;
						break;
					case -3:

						statDEF = round(statDEF * 1.25);
						defChange++;
						break;
					case -2:

						statDEF = round(statDEF * 1.34);
						defChange++;
						break;
					case -1:

						statDEF = round(statDEF / 0.67);
						defChange++;
						break;
					case 0:

						statDEF = round(statDEF * 1.5);
						defChange++;
						break;
					case 1:

						statDEF = round(statDEF / 0.75);
						defChange++;
						break;
					case 2:

						statDEF = round(statDEF * 1.25);
						defChange++;
						break;
					case 3:

						statDEF = round(statDEF * 1.2);
						defChange++;
						break;
					case 4:

						statDEF = round(statDEF * 1.1667);
						defChange++;
						break;
					case 5:

						statDEF = round(statDEF / 0.875);
						defChange++;
						break;
					case 6:
						return 7;

					default:
						break;
					}
				}
			}
			return power;
		}

		int Daemon::ChangeATKSPE(int power)
		{
			if (power < 0) 
			{
				for (int i = 0; i > power; i--) 
				{
					switch (atkSpeChange) 
					{
					case -6:
						return -7;
					case -5:

						statATKSPE = round(statATKSPE / 1.16);
						atkSpeChange--;
						break;
					case -4:

						statATKSPE = round(statATKSPE * 0.878788);
						atkSpeChange--;
						break;
					case -3:

						statATKSPE = round(statATKSPE * 0.825);
						atkSpeChange--;
						break;
					case -2:

						statATKSPE = round(statATKSPE / 1.25);
						atkSpeChange--;
						break;
					case -1:

						statATKSPE = round(statATKSPE / 1.34);
						atkSpeChange--;
						break;
					case 0:

						statATKSPE = round(statATKSPE * 0.67);
						atkSpeChange--;
						break;
					case 1:

						statATKSPE = round(statATKSPE / 1.5);
						atkSpeChange--;
						break;
					case 2:

						statATKSPE = round(statATKSPE * 0.75);
						atkSpeChange--;
						break;
					case 3:

						statATKSPE = round(statATKSPE / 1.25);
						atkSpeChange--;
						break;
					case 4:

						statATKSPE = round(statATKSPE / 1.2);
						atkSpeChange--;
						break;
					case 5:

						statATKSPE = round(statATKSPE / 1.1667);
						atkSpeChange--;
						break;
					case 6:

						statATKSPE = round(statATKSPE * 0.875);
						atkSpeChange--;
						break;
					default:
						break;
					}
				}
			}
			else 
			{

				for (int i = 0; i > power; i--) 
				{
					switch (atkSpeChange) 
					{
					case -6:

						statATKSPE = round(statATKSPE * 1.16);
						atkSpeChange++;
						break;
					case -5:

						statATKSPE = round(statATKSPE / 0.878788);
						atkSpeChange++;
						break;
					case -4:

						statATKSPE = round(statATKSPE / 0.825);
						atkSpeChange++;
						break;
					case -3:

						statATKSPE = round(statATKSPE * 1.25);
						atkSpeChange++;
						break;
					case -2:

						statATKSPE = round(statATKSPE * 1.34);
						atkSpeChange++;
						break;
					case -1:

						statATKSPE = round(statATKSPE / 0.67);
						atkSpeChange++;
						break;
					case 0:

						statATKSPE = round(statATKSPE * 1.5);
						atkSpeChange++;
						break;
					case 1:

						statATKSPE = round(statATKSPE / 0.75);
						atkSpeChange++;
						break;
					case 2:

						statATKSPE = round(statATKSPE * 1.25);
						atkSpeChange++;
						break;
					case 3:

						statATKSPE = round(statATKSPE * 1.2);
						atkSpeChange++;
						break;
					case 4:

						statATKSPE = round(statATKSPE * 1.1667);
						atkSpeChange++;
						break;
					case 5:

						statATKSPE = round(statATKSPE / 0.875);
						atkSpeChange++;
						break;
					case 6:
						return 7;
					default:
						break;
					}
				}
			}
			return power;
		}

		int Daemon::ChangeDEFSPE(int power)
		{
			if (power < 0) 
			{
				for (int i = 0; i > power; i--) 
				{
					switch (defSpeChange) 
					{
					case -6:
						return -7;
					case -5:

						statDEFSPE = round(statDEFSPE / 1.16);
						defSpeChange--;
						break;
					case -4:

						statDEFSPE = round(statDEFSPE * 0.878788);
						defSpeChange--;
						break;
					case -3:

						statDEFSPE = round(statDEFSPE * 0.825);
						defSpeChange--;
						break;
					case -2:

						statDEFSPE = round(statDEFSPE / 1.25);
						defSpeChange--;
						break;
					case -1:

						statDEFSPE = round(statDEFSPE / 1.34);
						defSpeChange--;
						break;
					case 0:

						statDEFSPE = round(statDEFSPE * 0.67);
						defSpeChange--;
						break;
					case 1:

						statDEFSPE = round(statDEFSPE / 1.5);
						defSpeChange--;
						break;
					case 2:

						statDEFSPE = round(statDEFSPE * 0.75);
						defSpeChange--;
						break;
					case 3:

						statDEFSPE = round(statDEFSPE / 1.25);
						defSpeChange--;
						break;
					case 4:

						statDEFSPE = round(statDEFSPE / 1.2);
						defSpeChange--;
						break;
					case 5:

						statDEFSPE = round(statDEFSPE / 1.1667);
						defSpeChange--;
						break;
					case 6:

						statDEFSPE = round(statDEFSPE * 0.875);
						defSpeChange--;
						break;
					default:
						break;
					}
				}
			}
			else
			{

				for (int i = 0; i > power; i--) 
				{
					switch (defSpeChange) 
					{
					case -6:

						statDEFSPE = round(statDEFSPE * 1.16);
						defSpeChange++;
						break;
					case -5:

						statDEFSPE = round(statDEFSPE / 0.878788);
						defSpeChange++;
						break;
					case -4:

						statDEFSPE = round(statDEFSPE / 0.825);
						defSpeChange++;
						break;
					case -3:

						statDEFSPE = round(statDEFSPE * 1.25);
						defSpeChange++;
						break;
					case -2:

						statDEFSPE = round(statDEFSPE * 1.34);
						defSpeChange++;
						break;
					case -1:

						statDEFSPE = round(statDEFSPE / 0.67);
						defSpeChange++;
						break;
					case 0:

						statDEFSPE = round(statDEFSPE * 1.5);
						defSpeChange++;
						break;
					case 1:

						statDEFSPE = round(statDEFSPE / 0.75);
						defSpeChange++;
						break;
					case 2:

						statDEFSPE = round(statDEFSPE * 1.25);
						defSpeChange++;
						break;
					case 3:

						statDEFSPE = round(statDEFSPE * 1.2);
						defSpeChange++;
						break;
					case 4:

						statDEFSPE = round(statDEFSPE * 1.1667);
						defSpeChange++;
						break;
					case 5:

						statDEFSPE = round(statDEFSPE / 0.875);
						defSpeChange++;
						break;
					case 6:
						return 7;
					default:
						break;
					}
				}
			}
			return power;
		}

		int Daemon::ChangeSPE(int power)
		{
			if (power < 0) 
			{
				for (int i = 0; i > power; i--) 
				{
					switch (speChange) 
					{
					case -6:
						return -7;
					case -5:

						statSPE = round(statSPE / 1.16);

						speChange--;
						break;
					case -4:

						statSPE = round(statSPE * 0.878788);
						speChange--;
						break;
					case -3:

						statSPE = round(statSPE * 0.825);
						speChange--;
						break;
					case -2:

						statSPE = round(statSPE / 1.25);
						speChange--;
						break;
					case -1:

						statSPE = round(statSPE / 1.34);
						speChange--;
						break;
					case 0:

						statSPE = round(statSPE * 0.67);
						speChange--;
						break;
					case 1:

						statSPE = round(statSPE / 1.5);
						speChange--;
						break;
					case 2:

						statSPE = round(statSPE * 0.75);
						speChange--;
						break;
					case 3:

						statSPE = round(statSPE / 1.25);
						speChange--;
						break;
					case 4:

						statSPE = round(statSPE / 1.2);
						speChange--;
						break;
					case 5:

						statSPE = round(statSPE / 1.1667);
						speChange--;
						break;
					case 6:

						statSPE = round(statSPE * 0.875);
						speChange--;
						break;
					default:
						break;
					}
				}
			}
			else 
			{

				for (int i = 0; i > power; i--) 
				{
					switch (speChange) 
					{
					case -6:

						statSPE = round(statSPE * 1.16);
						speChange++;
						break;
					case -5:

						statSPE = round(statSPE / 0.878788);
						speChange++;
						break;
					case -4:

						statSPE = round(statSPE / 0.825);
						speChange++;
						break;
					case -3:

						statSPE = round(statSPE * 1.25);
						speChange++;
						break;
					case -2:

						statSPE = round(statSPE * 1.34);
						speChange++;
						break;
					case -1:

						statSPE = round(statSPE / 0.67);
						speChange++;
						break;
					case 0:

						statSPE = round(statSPE * 1.5);
						speChange++;
						break;
					case 1:

						statSPE = round(statSPE / 0.75);
						speChange++;
						break;
					case 2:

						statSPE = round(statSPE * 1.25);
						speChange++;
						break;
					case 3:

						statSPE = round(statSPE * 1.2);
						speChange++;
						break;
					case 4:

						statSPE = round(statSPE * 1.1667);
						speChange++;
						break;
					case 5:

						statSPE = round(statSPE / 0.875);
						speChange++;
						break;
					case 6:
						return 7;
					default:
						break;
					}
				}
			}
			return power;
		}

		void Daemon::ResetAllOtherStats()
		{
			otherAtk = 1.0f;
			otherDef = 1.0f;
			otherAtkSpe = 1.0f;
			otherDefSpe = 1.0f;
			otherSpe = 1.0f;
			otherHP = 1.0f;
			otherMisc = 1.0f;
		}

		bool Daemon::SetStatus(Status status)
		{
			if (this->status == status) {
				return false;
			}
			else if (status != Status::NOTHING) { //If the OpMon already has a special status
				return false;
			}
			//Some status needs to edit the stats
			if (status == Status::BURNING) {
				ChangeATK(-1);
				atkChange++;
			}
			else if (status == Status::NOTHING && this->status == Status::BURNING) {
				ChangeATK(1);
				atkChange--;
			}
			else if (status == Status::PARALYSED) {
				ChangeSPE(-1);
				speChange++;
			}
			else if (status == Status::NOTHING && this->status == Status::PARALYSED) {
				ChangeSPE(1);
				speChange--;
			}
			this->status = status;
			return true;
		}


		#pragma endregion
	}
}
