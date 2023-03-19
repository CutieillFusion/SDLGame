#pragma once
#include "ECS.h"
#include "TransformComponent.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent() = default;
	ColliderComponent(std::vector<Vector3D> vertexs, bool isStatic)
	{
		ColliderComponent::mesh = vertexs;
		ColliderComponent::isStatic = isStatic;
	}

	void Initialize() override
	{
		transform = &entity->getComponent<TransformComponent>();

		movingMesh = mesh;
		for (int i = 0; i < movingMesh.size(); i++)
		{
			movingMesh[i].x += transform->position.x;
			movingMesh[i].y += transform->position.y;
		}
	}

	void Update() override
	{
		if (!isStatic)
		{
			movingMesh = mesh;
			for (int i = 0; i < movingMesh.size(); i++)
			{
				movingMesh[i].x += transform->position.x;
				movingMesh[i].y += transform->position.y;
			}
		}
	}

	void Render() override
	{

	}

	bool IsStatic() { return isStatic; }

	std::vector<Vector3D> movingMesh;
private:
	bool isStatic;
	TransformComponent* transform;
	std::vector<Vector3D> mesh;

	SDL_Texture* texture;
};
