#pragma once
#include "ECS.h"
#include "Vector.h"

class TransformComponent : public Component
{
public:
	TransformComponent() = default;
	TransformComponent(Vector2D position, Vector2D scale)
	{
		TransformComponent::position = position;
		TransformComponent::scale = scale;
		TransformComponent::rotationAngle = 0;
		TransformComponent::flipFlags = SDL_FLIP_NONE;
		destRect.x = 0;
		destRect.y = 0;
		destRect.w = 32;
		destRect.h = 32;
	}

	TransformComponent(Vector2D position, Vector2D scale, float rotationAngle, SDL_RendererFlip flipFlags)
	{
		TransformComponent::position = position;
		TransformComponent::scale = scale;
		TransformComponent::rotationAngle = 0;
		TransformComponent::flipFlags = flipFlags;
		destRect.x = 0;
		destRect.y = 0;
		destRect.w = 32;
		destRect.h = 32;
	}

	Vector2D position;
	Vector2D scale;
	float rotationAngle;
	SDL_RendererFlip flipFlags;
	SDL_FRect destRect;
};