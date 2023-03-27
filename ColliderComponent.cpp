#include "ColliderComponent.h"

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

void ColliderComponent::OnCollision()
{
}

void ColliderComponent::OnTrigger()
{
}

bool ColliderComponent::IsStatic()
{
	return isStatic;
}

bool ColliderComponent::IsTrigger()
{
	return isTrigger;
}
