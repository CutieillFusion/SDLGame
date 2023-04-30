#include "RectComponent.h"
#include "..\..\Globals.h"

RectComponent::RectComponent(Vector3D position, Vector3D scale) : localPosition(position), scale(scale), rotationAngle(0), flipFlags(SDL_FLIP_NONE)
{

}

RectComponent::RectComponent(Vector3D position, Vector3D scale, float rotationAngle, SDL_RendererFlip flipFlags) : localPosition(position), scale(scale), rotationAngle(rotationAngle), flipFlags(flipFlags)
{

}


void RectComponent::Initialize()
{
	entity->AddTag("UIElement");
	destRect = { 0,0, WORLD_SCALE, WORLD_SCALE };
}

void RectComponent::Update()
{
	localPosition += dPosition;
	dPosition = Vector3D();

	if (entity->HasParent())
	{
		position = entity->GetParent()->GetComponent<RectComponent>().position + localPosition;
	}
	else 
	{
		position = localPosition;
	}
}
