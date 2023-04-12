#include "Item.h"

namespace Daemon 
{
	namespace Model
	{
		Item::Item(std::string name, bool usable, bool onDaemon, std::unique_ptr<ItemEffect> daemonEffect, std::unique_ptr<ItemEffect> playerEffect, std::unique_ptr<ItemEffect> heldEffect)
			: name(name), usable(usable), daemonEffect(std::move(daemonEffect)), playerEffect(std::move(playerEffect)), heldEffect(std::move(heldEffect))
		{ }

		std::unique_ptr<std::vector<std::string>> Item::use(Daemon* daemon, int& itemCount)
		{
			if (daemonEffect == nullptr) 
			{
				return nullptr;
			}

			bool result = (*daemonEffect)(daemon);
			itemCount = (result ? itemCount - 1 : itemCount);
			return daemonEffect->GetDialog();
		}

		std::unique_ptr<std::vector<std::string>> Item::use(Player* player, int& itemCount) 
		{
			if (playerEffect == nullptr) 
			{
				return nullptr;
			}

			bool result = (*playerEffect)(player);
			itemCount = (result ? itemCount - 1 : itemCount);
			return playerEffect->GetDialog();
		}

		void Item::updateHeld(Turn& owner, Turn& opponent, int& itemCount) 
		{
			if (heldEffect == nullptr) 
			{
				return;
			}

			bool result = (*heldEffect)(owner, opponent);
			itemCount = (result ? itemCount - 1 : itemCount);
		}

		namespace Items
		{
			HpHealEffect::HpHealEffect(int hpHealed)
				: hpHealed(hpHealed)
			{
			}

			bool HpHealEffect::operator()(Daemon* daemon)
			{
				dialog = std::make_unique<std::vector<std::string>>();
				bool toReturn = true;
				if (daemon->GetHP() == daemon->GetStatHP())
				{
					//dialog->push_back(Utils::OpString::quickString("items.dialog.heal.fullHP", { opmon->getNickname() }));
					toReturn = false;
				}
				else if (daemon->GetHP() <= 0)
				{
					//dialog->push_back(Utils::OpString::quickString("items.dialog.heal.healKO", { opmon->getNickname() }));
					toReturn = false;
				}
				else
				{
					int oldHP = daemon->GetHP();
					daemon->Heal(hpHealed);
					int healed = daemon->GetHP() - oldHP;
					//dialog->push_back(Utils::OpString::quickString("items.dialog.heal.healHP", { opmon->getNickname(), std::to_string(healed) }));
				}

				//dialog->push_back(Utils::OpString::quickString("void"));
				//dialog->push_back(Utils::OpString::quickString("void"));

				return toReturn;
			}
		}
	}
}