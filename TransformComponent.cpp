#include "TransformComponent.h"
#include "Globals.h"

TransformComponent::TransformComponent(Vector3D position, Vector3D scale) : localPosition(position), scale(scale), rotationAngle(0), flipFlags(SDL_FLIP_NONE)
{

}

TransformComponent::TransformComponent(Vector3D position, Vector3D scale, float rotationAngle, SDL_RendererFlip flipFlags) : localPosition(position), scale(scale), rotationAngle(rotationAngle), flipFlags(flipFlags)
{

}

void TransformComponent::Initialize()
{
	entity->AddTag("WorldElement");
	destRect = { 0,0, WORLD_SCALE, WORLD_SCALE };
}

void TransformComponent::Update()
{
	while (std::fabs(dPosition.x) > MinMov) 
	{ 
		if (dPosition.x > 0) 
		{
			localPosition.x += MinMov;
			dPosition.x -= MinMov;
		}
		else if(dPosition.x < 0)
		{
			localPosition.x -= MinMov;
			dPosition.x += MinMov;
		}
	}
	while (std::fabs(dPosition.y) > MinMov)
	{
		if (dPosition.y > 0)
		{
			localPosition.y += MinMov;
			dPosition.y -= MinMov;
		}
		else if (dPosition.y < 0)
		{
			localPosition.y -= MinMov;
			dPosition.y += MinMov;
		}
	}

	//localPosition += dPosition;
	dPosition = Vector3D();

	if (entity->HasParent())
	{
		position = entity->GetParent()->getComponent<TransformComponent>().position + localPosition;
	}
	else
	{
		position = localPosition;
	}
}
