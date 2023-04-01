#pragma once
#include "ECS.h"
#include "Globals.h"
#include "Vector.h"

class RectComponent : public Component
{
public:
	RectComponent() = default;
	RectComponent(Vector3D position, Vector3D scale);
	RectComponent(Vector3D position, Vector3D scale, float rotationAngle, SDL_RendererFlip flipFlags);

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

