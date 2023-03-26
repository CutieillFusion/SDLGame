#include "ImageRendererComponent.h"

ImageRendererComponent::ImageRendererComponent(Vector3D position, Vector3D scale) : position(position), scale(scale)
{
	destRect.x = position.x;
	destRect.y = position.y;
	destRect.w = scale.x;
	destRect.h = scale.y;
}

void ImageRendererComponent::Initialize()
{
}

void ImageRendererComponent::Update()
{	
}

void ImageRendererComponent::Render()
{
	SDL_RenderCopyF(Game::renderer, sprites->GetCurrentTexture(), nullptr, &destRect);
}
