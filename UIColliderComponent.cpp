#include "UIColliderComponent.h"

UIColliderComponent::UIColliderComponent(std::vector<Vector3D> vertexs)
{
	UIColliderComponent::mesh = vertexs;
}

void UIColliderComponent::Initialize()
{
	rect = &entity->getComponent<RectComponent>();

	movingMesh = mesh;
	for (int i = 0; i < movingMesh.size(); i++)
	{
		movingMesh[i].x *= rect->scale.x;
		movingMesh[i].y *= rect->scale.y;
		movingMesh[i].x += rect->position.x - Game::camera.x;
		movingMesh[i].y += rect->position.y - Game::camera.y;
	}
}

void UIColliderComponent::Update()
{
	movingMesh = mesh;
	for (int i = 0; i < movingMesh.size(); i++)
	{
		movingMesh[i].x *= rect->scale.x;
		movingMesh[i].y *= rect->scale.y;
		movingMesh[i].x += rect->position.x - Game::camera.x;
		movingMesh[i].y += rect->position.y - Game::camera.y;
	}
}

void UIColliderComponent::Render()
{
}
