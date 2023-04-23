#include "ColliderComponent.h"
#include "TrainerComponent.h"
#include "Globals.h"
#include "Battle.h"

ColliderComponent::ColliderComponent(std::vector<Vector3D> vertexs, bool isStatic, bool isTrigger)
{
	ColliderComponent::mesh = vertexs;
	ColliderComponent::isStatic = isStatic;
	ColliderComponent::isTrigger = isTrigger;
}

void ColliderComponent::Initialize()
{
	transform = &entity->getComponent<TransformComponent>();

	movingMesh = mesh;
	for (int i = 0; i < movingMesh.size(); i++)
	{
		movingMesh[i].x *= transform->scale.x;
		movingMesh[i].y *= transform->scale.y;
		movingMesh[i].x += transform->position.x - Game::camera.x;
		movingMesh[i].y += transform->position.y - Game::camera.y;
	}
}

void ColliderComponent::Update()
{
	movingMesh = mesh;
	for (int i = 0; i < movingMesh.size(); i++)
	{
		movingMesh[i].x *= transform->scale.x;
		movingMesh[i].y *= transform->scale.y;
		movingMesh[i].x += transform->position.x - Game::camera.x;
		movingMesh[i].y += transform->position.y - Game::camera.y;
	}
}

void ColliderComponent::Render()
{
}

void ColliderComponent::OnCollision(Entity* entity)
{
}

void ColliderComponent::OnTrigger(Entity* entity)
{
	if (entity->HasTag("Player")) 
	{
		if (entity->getComponent<TrainerComponent>().CanBattle() && this->entity->GetParent()->getComponent<TrainerComponent>().CanBattle()) 
		{
			auto trainerTeam = this->entity->GetParent()->getComponent<TrainerComponent>().team;
			auto playerTeam = entity->getComponent<TrainerComponent>().team;
			auto battle = new Daemon::Model::Battle(playerTeam, trainerTeam, 0, 0);
		}
	}
}

bool ColliderComponent::IsStatic()
{
	return isStatic;
}

bool ColliderComponent::IsTrigger()
{
	return isTrigger;
}
