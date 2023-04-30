#pragma once
#include "..\ECS.h"
#include "..\..\Game.h"
#include "TransformComponent.h"
#include "..\..\Globals.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent() = default;
	ColliderComponent(std::vector<Vector3D> vertexs, bool isStatic, bool isTrigger);

	void Initialize() override;
	void Update() override;
	void Render() override;

	void OnCollision(Entity* entity);
	void OnTrigger(Entity* entity);

	bool IsStatic();
	bool IsTrigger();

	std::vector<Vector3D> movingMesh;
private:
	bool isStatic;
	bool isTrigger;
	TransformComponent* transform;
	std::vector<Vector3D> mesh;
};