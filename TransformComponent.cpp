#include "TransformComponent.h"

TransformComponent::TransformComponent(Vector3D position, Vector3D scale)
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

TransformComponent::TransformComponent(Vector3D position, Vector3D scale, float rotationAngle, SDL_RendererFlip flipFlags)
{
	TransformComponent::position = position;
	TransformComponent::scale = scale;
	TransformComponent::rotationAngle = rotationAngle;
	TransformComponent::flipFlags = flipFlags;
	destRect.x = 0;
	destRect.y = 0;
	destRect.w = WORLD_SCALE;
	destRect.h = WORLD_SCALE;
}

void TransformComponent::Update()
{
	position += dPosition;
	dPosition = Vector3D();
}
