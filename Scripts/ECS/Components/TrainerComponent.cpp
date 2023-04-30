#include "TrainerComponent.h"

TrainerComponent::TrainerComponent(Daemon::Model::DaeTeam* team) : team(team)
{

}

void TrainerComponent::Initialize()
{
	entity->AddTag("Trainer");
}

void TrainerComponent::OnTrigger(Entity* entity)
{
}
