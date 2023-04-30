#pragma once

#include "CurveExp.h"
#include "Enums.h"
#include <iostream>
#include <vector>

namespace Daemon 
{
	namespace Model 
	{

		class Evolution;

		class Species
		{
		protected:
		private:
			std::string name;
			int daedexNumber;
			Species* evolution;
			Evolution* evolType;
			Type type1;
			Type type2;
			std::string daedexEntry;
			float height;
			float weight;
			unsigned int baseAtk;
			unsigned int baseDef;
			unsigned int baseAtkSpe;
			unsigned int baseDefSpe;
			unsigned int baseSpe;
			unsigned int baseHP;

			//EXP Curve
			CurveExp* curve;
			//Total EXP
			int expMax;
			//Represents Dropped EV's
			std::vector<Stats> EVgiven;
			//Dropped EXP
			unsigned int expGiven;

			int captureRate;
		public:
			Species(Species&& other) noexcept;

			virtual ~Species();
            Species(unsigned int atk, unsigned int def, unsigned int atkSpe, unsigned int defSpe, unsigned int spe, unsigned int hp, std::string name, Type type1, Type type2, Evolution* evolType, std::vector<Stats> EVGiven, float height, float weight, std::string daedexEntry, unsigned int expGiven, int expMax, int captureRate, int daedexNumber);
            
            unsigned int GetBaseAtk() const 
            {
                return baseAtk;
            }

            unsigned int GetBaseDef() const 
            {
                return baseDef;
            }

            unsigned int GetBaseAtkSpe() const 
            {
                return baseAtkSpe;
            }

            unsigned int GetBaseDefSpe() const 
            {
                return baseDefSpe;
            }

            unsigned int GetBaseSpe() const 
            {
                return baseSpe;
            }

            unsigned int GetBaseHP() const 
            {
                return baseHP;
            }

            std::string GetName() const 
            {
                return name;
            }


            int GetCaptureRate() const 
            {
                return captureRate;
            }

            Type GetType1() const 
            {
                return type1;
            }

            Type GetType2() const
            {
                return type2;
            }

            unsigned int GetExp() const 
            {
                return expGiven;
            }

            int GetExpMax() const
            {
                return expMax;
            }

            Evolution* GetEvolType() const 
            {
                return evolType;
            }

            CurveExp* GetCurve() const 
            {
                return curve;
            }

            Species* GetEvolution() const 
            {
                return evolution;
            }

            float GetWeight() const
            {
                return weight;
            }

            float GetHeight() const 
            {
                return height;
            }

            std::vector<Stats> GetEv() const 
            {
                return EVgiven;
            }

            int GetDaedexNumber() const
            {
                return this->daedexNumber;
            }
		};
	}
}

