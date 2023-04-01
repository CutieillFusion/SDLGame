#include "ImageRendererComponent.h"
#include "Globals.h"

void ImageRendererComponent::Initialize()
{
	rect = &entity->getComponent<RectComponent>();
	sprites = &entity->getComponent<SpriteComponent>();

	destRect = { rect->position.x, rect->position.y, rect->scale.x, rect->scale.y };
}

void ImageRendererComponent::Update()
{	
	destRect = { rect->position.x, rect->position.y, rect->scale.x, rect->scale.y };
}

void ImageRendererComponent::Render()
{
	SDL_RenderCopyF(Game::renderer, sprites->GetCurrentTexture(), nullptr, &destRect);
}
