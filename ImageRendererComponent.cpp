#include "ImageRendererComponent.h"

ImageRendererComponent::ImageRendererComponent(std::string id, Vector3D position, Vector3D scale) : position(position), scale(scale)
{
	texture = Game::assets->GetTexture(id);

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
	SDL_RenderCopyF(Game::renderer, texture, nullptr, &destRect);
}
