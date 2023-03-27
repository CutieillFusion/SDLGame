#include "RectComponent.h"

RectComponent::RectComponent(Vector3D position, Vector3D scale)
{
	RectComponent::position = position;
	RectComponent::scale = scale;
	RectComponent::rotationAngle = 0;
	RectComponent::flipFlags = SDL_FLIP_NONE;
	destRect = { 0,0, WORLD_SCALE, WORLD_SCALE };

}

RectComponent::RectComponent(Vector3D position, Vector3D scale, float rotationAngle, SDL_RendererFlip flipFlags)
{
	RectComponent::position = position;
	RectComponent::scale = scale;
	RectComponent::rotationAngle = rotationAngle;
	RectComponent::flipFlags = flipFlags;
	destRect = { 0,0, WORLD_SCALE, WORLD_SCALE };
}

void RectComponent::Update()
{
	position += dPosition;
	dPosition = Vector3D();
}
