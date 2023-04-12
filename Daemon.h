#pragma once

#include "Utils.h"
#include "Nature.h"
#include "Species.h"
#include <cmath>
#include <iostream>
#include <string>

#define MAX_EV 252
#define MAX_EVS 510
#define MAX_IV 31
namespace Daemon 
{
	namespace Model 
	{
		class Species;

		class Attack;

		class Item;

		class Daemon
		{
		protected:
		private:
			std::string nickname;
			int atkIV = Utils::randU(MAX_IV + 1);
			int defIV = Utils::randU(MAX_IV + 1);
			int atkSpeIV = Utils::randU(MAX_IV + 1);
			int defSpeIV = Utils::randU(MAX_IV + 1);
			int speIV = Utils::randU(MAX_IV + 1);
			int hpIV = Utils::randU(MAX_IV + 1);
			int atkEV = 0;
			int defEV = 0;
			int atkSpeEV = 0;
			int defSpeEV = 0;
			int speEV = 0;
			int hpEV = 0;

			//General Stats
			int statATK;
			int statDEF;
			int statATKSPE;
			int statDEFSPE;
			int statSPE;
			//Other Stats
			int statEVA;
			float statACC;
			int statHP;
			int statLove;
			//Variables Indicating stats variation level
			int atkChange = 0;
			int defChange = 0;
			int defSpeChange = 0;
			int atkSpeChange = 0;
			int evaChange = 0;
			int accChange = 0;
			int speChange = 0;

            //For Moves like protect or light-screen
            float otherAtk = 1.0f;
            float otherDef = 1.0f;
            float otherAtkSpe = 1.0f;
            float otherDefSpe = 1.0f;
            float otherHP = 1.0f;
            float otherSpe = 1.0f;
            float otherMisc = 1.0f;
			const Species* species;
			int level;

			std::vector<Attack*> attacks;

			Nature nature;

			int HP;
			
			Status status = Status::NOTHING;
			Type type1;
			Type type2;

			int exp;
			int toNextLevel;
			float expBoost;

			Item* held;

			int captureRate;

			unsigned int confusedCD = 0;
			unsigned int sleepingCD = 0;
		public:
			bool initialized = true;

			bool confused = false;
			bool afraid = false;
			bool inLove = false;
			bool vampigraine = false;
			bool malediction = false;

			virtual ~Daemon();

			Daemon(const std::string& nickname, const Species* species, int level, const std::vector<Attack*>& attacks, Nature nature);

			Daemon() 
			{
				initialized = true;
			}

            int GetConfusedCD() {
                return confusedCD;
            }

            int GetSleepingCD() {
                return sleepingCD;
            }

            void PassCD(bool sleep);

            void GoToSleep() {
                sleepingCD = Utils::randU(3);
                SetStatus(Status::SLEEPING);
            }

            void MakeConfused() {
                confused = true;
                confusedCD = Utils::randU(4);
            }

            /**Returns true if the Daemon is well captured*/
            bool Captured(Item const& OpBox);

            /** Set the stat given*/
            void SetStat(Stats stat, int newStat);

            /**Method called when leveling up*/
            void LevelUp();

            bool IsHoldingItem() const {
                return (held == nullptr);
            }

            /**Method called when the Daemon wins*/
            int Win(Daemon const& defeated);

            /**Recalculates the stats*/
            void CalcStats();

            /**Method called when an item is used. Returns true if the item must be deleted from the player's inventory*/
            bool ItemUsed(Item* used);

            Item* Hold(Item* item);
            /**Method called during an exchange*/
            void Traded();

            /**Method called when the OpMon is about to evolve*/
            void Evolve();

            /**Allows to completely change the OPMon*/
            void SetStats(int stats[], Attack* attacks[], const Species& species, Type types[]);

            /**Attack the OpMon (he looses HP)*/
            void Attacked(int hpLost);

            /**Changes the STAT depending of the power given. It doesn't edit directly the variable, it decreases or increases (Depending of the power's sign) by multiplicating it.*/
            int ChangeATK(int power);
            int ChangeACC(int power);
            int ChangeEVA(int power);
            int ChangeDEF(int power);
            int ChangeATKSPE(int power);
            int ChangeDEFSPE(int power);
            int ChangeSPE(int power);

            void SetOtherAtk(float stat) { otherAtk = stat; }
            void SetOtherDef(float stat) { otherDef = stat; }
            void SetOtherAtkSpe(float stat) { otherAtkSpe = stat; }
            void SetOtherDefSpe(float stat) { otherDefSpe = stat; }
            void SetOtherSpe(float stat) { otherSpe = stat; }
            void SetOtherHP(float stat) { otherHP = stat; }
            void SetOtherMisc(float stat) { otherMisc = stat; }

            float GetOtherAtk() { return otherAtk; }
            float GetOtherDef() { return otherDef; }
            float GetOtherAtkSpe() { return otherAtkSpe; }
            float GetOtherDefSpe() { return otherDefSpe; }
            float GetOtherSpe() { return otherSpe; }
            float GetOtherHP() { return otherHP; }
            float GetOtherMisc() { return otherMisc; }

            void ResetAllOtherStats();

            void SetAtkIV(int iv) { atkIV = iv > MAX_IV ? MAX_IV : iv; }
            void SetDefIV(int iv) { defIV = iv > MAX_IV ? MAX_IV : iv; }
            void SetAtkSpeIV(int iv) { atkSpeIV = iv > MAX_IV ? MAX_IV : iv; }
            void SetDefSpeIV(int iv) { defSpeIV = iv > MAX_IV ? MAX_IV : iv; }
            void SetSpeIV(int iv) { speIV = iv > MAX_IV ? MAX_IV : iv; }
            void SetHPIV(int iv) { hpIV = iv > MAX_IV ? MAX_IV : iv; }

            int GetAtkIV() { return atkIV; }
            int GetDefIV() { return defIV; }
            int GetAtkSpeIV() { return atkSpeIV; }
            int GetDefSpeIV() { return defSpeIV; }
            int GetSpeIV() { return speIV; }
            int GetHPIV() { return hpIV; }

            Status GetStatus() {
                return status;
            }

            /**Changes OpMon's status*/
            bool SetStatus(Status status);

            int GetStatHP() const {
                return statHP;
            }

            int GetStatLove() const {
                return statLove;
            }

            int GetHP() const {
                return HP;
            }

            std::string& GetNickname() {
                return nickname;
            }

            std::string* GetNicknamePtr() {
                return &nickname;
            }

            /**Heals the OPMon (Opposite of attack() )*/
            void Heal(int HP);

            int GetLevel() const {
                return level;
            }

            std::vector<Attack*> GetAttacks() {
                return attacks;
            }

            int GetStatEVA() const {
                return statEVA;
            }

            int GetStatACC() const {
                return statACC;
            }

            void GetEvs(Daemon const& defeated);

            Type GetType1() const {
                return type1;
            }

            Type GetType2() const {
                return type2;
            }

            void SetType1(Type type);

            void SetType2(Type type);

            int GetStatATK() const {
                return statATK;
            }

            int GetStatATKSPE() const {
                return statATKSPE;
            }

            int GetStatDEF() const {
                return statDEF;
            }

            int GetStatDEFSPE() const {
                return statDEFSPE;
            }

            int GetStatSPE() const {
                return statSPE;
            }

            const Species& GetSpecies() const {
                return *species;
            }

            //Warning! The == and != operators do not compare two OPMons! They compare whether OPMons are "initialized" or not (see initialized))
            //Yup, I should change this in the future (TODO)
            bool operator==(Daemon const& a) {
                return (initialized == a.initialized);
            }

            bool operator!=(Daemon const& a) {
                return !(initialized == a.initialized);
            }

            Item* ItemHeld() const {
                return held;
            }
		};
	}
}

