#pragma once
#include "ECS.h"
#include "Vector.h"
#include "Game.h"

class TransformComponent : public Component
{
public:
	TransformComponent() = default;
	TransformComponent(Vector3D position, Vector3D scale)
	{
		TransformComponent::position = position;
		TransformComponent::scale = scale;
		TransformComponent::rotationAngle = 0;
		TransformComponent::flipFlags = SDL_FLIP_NONE;
		destRect.x = 0;
		destRect.y = 0;
		destRect.w = WORLD_SCALE;
		destRect.h = WORLD_SCALE;
	}

	TransformComponent(Vector3D position, Vector3D scale, float rotationAngle, SDL_RendererFlip flipFlags)
	{
		TransformComponent::position = position;
		TransformComponent::scale = scale;
		TransformComponent::rotationAngle = 0;
		TransformComponent::flipFlags = flipFlags;
		destRect.x = 0;
		destRect.y = 0;
		destRect.w = WORLD_SCALE;
		destRect.h = WORLD_SCALE;
	}

	void Update() override 
	{
		position += dPosition;
		dPosition = Vector3D();
	}

	Vector3D dPosition;
	Vector3D position;
	Vector3D scale;
	float rotationAngle;
	SDL_RendererFlip flipFlags;
	SDL_FRect destRect;
};