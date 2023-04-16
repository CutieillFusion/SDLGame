#include "DaemonTeam.h"
namespace Daemon
{
	namespace Model
	{
		DaeTeam::~DaeTeam()
		{
			for (Daemon* dae : daeTeam) {
				if (dae != nullptr) {
					delete(dae);
				}
			}
		}

		DaeTeam::DaeTeam(std::string const& name)
		{
			this->name = name;
		}

		void DaeTeam::Heal()
		{
			for (auto daemon : daeTeam) 
			{
				daemon->Heal(daemon->GetStatHP());
				daemon->confused = false;
				daemon->SetStatus(Status::NOTHING);
			}
		}
		bool DaeTeam::AddDaeMon(Daemon* toAdd)
		{
			if (daeTeam.size() < 6) 
			{
				daeTeam.push_back(toAdd);
				return true;
			}
			else 
			{
				return false;
			}
		}
		Daemon* DaeTeam::RemoveDae(int index)
		{
			if (daeTeam.size() == 1)// Won't remove if you only have 1 Daemon
			{
				return nullptr;
			}

			Daemon* toReturn = daeTeam[index];
			daeTeam.erase(daeTeam.begin() + index);
			return toReturn;
		}
		Daemon* DaeTeam::operator[](int index) const
		{
			return daeTeam[index];
		}
		bool DaeTeam::IsKo() const
		{
			unsigned int ko = 0;
			for (auto daemon : daeTeam) 
			{
				if (daemon->GetHP() <= 0) 
				{
					ko++;
				}
			}
			return (ko == daeTeam.size());
		}
		int DaeTeam::GetSize() const
		{
			return daeTeam.size();
		}
	}
}
