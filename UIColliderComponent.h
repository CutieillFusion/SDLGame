#pragma once
#include "ECS.h"
#include "Game.h"
#include "RectComponent.h"
#include "Vector.h"

class UIColliderComponent : public Component
{
public:
	UIColliderComponent() = default;
	UIColliderComponent(std::vector<Vector3D> vertexs);

	void Initialize() override;
	void Update() override;
	void Render() override;

	std::vector<Vector3D> movingMesh;
	bool colliding;
private:
	RectComponent* rect;
	std::vector<Vector3D> mesh;
};

