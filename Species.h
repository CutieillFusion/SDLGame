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
            
            unsigned int getBaseAtk() const 
            {
                return baseAtk;
            }

            unsigned int getBaseDef() const 
            {
                return baseDef;
            }

            unsigned int getBaseAtkSpe() const 
            {
                return baseAtkSpe;
            }

            unsigned int getBaseDefSpe() const 
            {
                return baseDefSpe;
            }

            unsigned int getBaseSpe() const 
            {
                return baseSpe;
            }

            unsigned int getBaseHP() const 
            {
                return baseHP;
            }

            std::string getName() const 
            {
                return name;
            }


            int getCaptureRate() const 
            {
                return captureRate;
            }

            Type getType1() const 
            {
                return type1;
            }

            Type getType2() const
            {
                return type2;
            }

            unsigned int getExp() const 
            {
                return expGiven;
            }

            int getExpMax() const
            {
                return expMax;
            }

            Evolution* getEvolType() const 
            {
                return evolType;
            }

            CurveExp* getCurve() const 
            {
                return curve;
            }

            Species* getEvolution() const 
            {
                return evolution;
            }

            float getWeight() const
            {
                return weight;
            }

            float getHeight() const 
            {
                return height;
            }

            std::vector<Stats> getEv() const 
            {
                return EVgiven;
            }

            int getDaedexNumber() const
            {
                return this->daedexNumber;
            }
		};
	}
}

