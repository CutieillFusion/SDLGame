#pragma once
#include "Daemon.h"
#include <memory>
#include <vector>

namespace Daemon 
{
	namespace Model
	{
		struct Turn;
		typedef Turn Turn;

		class Player;

		class ItemEffect
		{
		public:
			ItemEffect() {}

			bool operator()(Daemon* daemon) { return false; }//Used on Daemon
			bool operator()(Player* player) { return false; }//Used on the Field
			bool operator()(Turn& owner, Turn& opponent) { return false; }//Effect of Held Item on Turn

			std::unique_ptr<std::vector<std::string>> GetDialog() { return std::move(dialog); }
		protected:
			std::unique_ptr<std::vector<std::string>> dialog = std::make_unique<std::vector<std::string>>();
		};

		class Item
		{
		public:
			Item(std::string name, bool usable, bool onDaemon, std::unique_ptr<ItemEffect> daemonEffect, std::unique_ptr<ItemEffect> playerEffect, std::unique_ptr<ItemEffect> heldEffect);

			~Item() = default;

			std::unique_ptr<std::vector<std::string>> Use(Daemon* daemon, int& itemCount);
			std::unique_ptr<std::vector<std::string>> Use(Player* player, int& itemCount);
			void UpdateHeld(Turn& owner, Turn& opponent, int& itemCount);

		protected:
			std::string name;
			bool usable;
			bool onDaemon;
			std::unique_ptr<ItemEffect> daemonEffect;
			std::unique_ptr<ItemEffect> playerEffect;
			std::unique_ptr<ItemEffect> heldEffect;
		};

		namespace Items {

			//Inherited Item Class Code Space
			class HpHealEffect : public ItemEffect
			{
			public:
				HpHealEffect(int hpHealed);

				bool operator()(Daemon* daemon);

			protected:
				const int hpHealed;
			};

		}
	}
}


