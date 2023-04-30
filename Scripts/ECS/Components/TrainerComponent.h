#pragma once

#include "..\..\Globals.h"
#include "..\ECS.h"
#include "..\..\Daemon\DaemonTeam.h"

class TrainerComponent : public Component
{
public:
	TrainerComponent() = default;
	TrainerComponent(Daemon::Model::DaeTeam* team);

	void Initialize() override;

	void OnTrigger(Entity* entity);
	bool CanBattle() { return !team->IsKo(); }

	Daemon::Model::DaeTeam* team;
};

