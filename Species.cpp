#include "Species.h"
#include "Evolution.h"
#include "evolutions.h"

namespace Daemon 
{
	namespace Model 
	{
        Species::Species(Species&& other) noexcept {
            name = other.name;
            daedexNumber = other.daedexNumber;
            evolution = other.evolution;
            evolType = other.evolType;
            type1 = other.type1;
            type2 = other.type2;
            daedexEntry = other.daedexEntry;
            height = other.height;
            weight = other.weight;
            baseAtk = other.baseAtk;
            baseDef = other.baseDef;
            baseAtkSpe = other.baseAtkSpe;
            baseDefSpe = other.baseDefSpe;
            baseSpe = other.baseSpe;
            baseHP = other.baseHP;
            curve = other.curve;
            expMax = other.expMax;
            EVgiven = other.EVgiven;
            expGiven = other.expGiven;
            captureRate = other.captureRate;

            other.evolType = nullptr;
        }

        Species::~Species() {
            delete(evolType);
            delete(curve);
        }

#pragma GCC diagnostic ignored "-Wunused-parameter"
        Species::Species(unsigned int atk, unsigned int def, unsigned int atkSpe, unsigned int defSpe, unsigned int spe, unsigned int hp, std::string name, Type type1, Type type2, Evolution* evolType, std::vector<Stats> EVGiven, float height, float weight, std::string daedexEntry, unsigned int expGiven, int expMax, int captureRate, int daedexNumber) {
            this->daedexNumber = daedexNumber;
            this->captureRate = captureRate;
            this->baseAtk = atk;
            this->baseDef = def;
            this->baseAtkSpe = atkSpe;
            this->baseDefSpe = defSpe;
            this->baseSpe = spe;
            this->name = name;
            this->baseHP = hp;
            this->type1 = type1;
            this->type2 = type2;
            this->evolType = evolType;
            this->evolution = ((evolType != nullptr) ? this->evolType->GetEvolution() : nullptr);
            this->weight = weight;
            this->height = height;
            this->daedexEntry = daedexEntry;
            this->expGiven = expGiven;
            this->EVgiven = EVGiven;
            this->expMax = expMax;

            //The curve is found according to the max Daemon's exp.
            switch (this->expMax) {
            case 800000:
                curve = new CurveExpQuick();
                break;
            case 1000000:
                curve = new CurveExpNormal();
                break;
            case 1059860:
                curve = new CurveExpParabolic();
                break;
            case 1250000:
                curve = new CurveExpSlow();
                break;
            case 600000:
                curve = new CurveExpErratic();
                break;
            case 1640000:
                curve = new CurveExpFluctuating();
                break;
            default:
                curve = new CurveExpNormal();
            }
        }
#pragma GCC diagnostic pop
	}
}
