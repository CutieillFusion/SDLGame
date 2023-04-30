#include "ColliderComponent.h"
#include "TrainerComponent.h"
#include "..\..\Daemon\Battle.h"
#include "..\..\Globals.h"

ColliderComponent::ColliderComponent(std::vector<Vector3D> vertexs, bool isStatic, bool isTrigger)
{
	ColliderComponent::mesh = vertexs;
	ColliderComponent::isStatic = isStatic;
	ColliderComponent::isTrigger = isTrigger;
}

void ColliderComponent::Initialize()
{
	transform = &entity->GetComponent<TransformComponent>();

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
		if (entity->GetComponent<TrainerComponent>().CanBattle() && this->entity->GetParent()->GetComponent<TrainerComponent>().CanBattle()) 
		{
			auto trainerTeam = this->entity->GetParent()->GetComponent<TrainerComponent>().team;
			auto playerTeam = entity->GetComponent<TrainerComponent>().team;
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
