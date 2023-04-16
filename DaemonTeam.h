#pragma once
#include <iostream>
#include <vector>
#include "Daemon.h"

namespace Daemon 
{
	namespace Model
	{
        class Daemon;

        /**
            Class defining a OpMon team, used by the player and the trainers.
        */
        class DaeTeam {
        public:
            ~DaeTeam();
            DaeTeam(std::string const& name);

            std::string GetName() const 
            {
                return name;
            }
            std::vector<Daemon*> GetDaeTeam() 
            {
                return daeTeam;
            }
            /**Heals all the team*/
            void Heal();
            /**
           Adds an OpMon to the team. Returns false if the team is full and the OpMon is not added.
            */
            bool AddDaeMon(Daemon* toAdd);
            /**Delete the OpMon with the index specified*/
            Daemon* RemoveDae(int index);
            /**
           Returns the OpMon in the index specified.
            */
            Daemon* operator[](int index) const;
            Daemon* GetDae(int id) const {
                return (*this)[id];
            }
            /**Returns true if the team is K.O*/
            bool IsKo() const;
            int GetSize() const;
            void operator+=(Daemon* pkmn) {
                AddDaeMon(pkmn);
            }
            void operator-=(int id) {
                RemoveDae(id);
            }
            //void Save();

        private:
            std::vector<Daemon*> daeTeam;
            std::string name;
            DaeTeam(DaeTeam const&) = delete;
        };
	}
}

