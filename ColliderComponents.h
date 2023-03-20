#pragma once
#include "ECS.h"
#include "Game.h"
#include "TransformComponent.h"
#include "Vector.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent() = default;
	ColliderComponent(std::vector<Vector3D> vertexs, bool isStatic, bool isTrigger)
	{
		ColliderComponent::mesh = vertexs;
		ColliderComponent::isStatic = isStatic;
		ColliderComponent::isTrigger = isTrigger;
	}

	void Initialize() override
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

	void Update() override
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

	void Render() override 
	{

	}

	//Add Enter, Stay, Exit for Collision and Trigger;

	void OnTrigger() 
	{
		
	}

	bool IsStatic() { return isStatic; }
	bool IsTrigger() { return isTrigger; }

	std::vector<Vector3D> movingMesh;
private:
	bool isStatic;
	bool isTrigger;
	TransformComponent* transform;
	std::vector<Vector3D> mesh;

	SDL_Texture* texture;
};
