#include "CameraComponent.h"
#include "TransformComponent.h"

CameraComponent* CameraComponent::camera = nullptr;

CameraComponent::CameraComponent()
{
	camera = this;
}

void CameraComponent::Initialize()
{

}

void CameraComponent::Update()
{
	cameraOffset = entity->getComponent<TransformComponent>().position;
}

void CameraComponent::Render()
{
}
