#pragma once
#include "ECS.h"
#include "Vector.h"

#define WORLD_SCALE 32

class TransformComponent : public Component
{
public:
	TransformComponent() = default;
	TransformComponent(Vector3D position, Vector3D scale);
	TransformComponent(Vector3D position, Vector3D scale, float rotationAngle, SDL_RendererFlip flipFlags);

	void Initialize() override;
	void Update() override;

	Vector3D dPosition;
	Vector3D position;
	Vector3D localPosition;
	Vector3D scale;
	float rotationAngle;
	SDL_RendererFlip flipFlags;
	SDL_FRect destRect{};
};