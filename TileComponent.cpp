#include "TileComponent.h"

void TileComponent::Initialize()
{
	srcRect = { 0, 0, WORLD_SCALE, WORLD_SCALE };

	transform = &entity->getComponent<TransformComponent>();
	sprites = &entity->getComponent<SpriteComponent>();
}

void TileComponent::Update()
{
	transform->destRect.x = (transform->position.x - Game::camera.x) * WORLD_SCALE;
	transform->destRect.y = (transform->position.y - Game::camera.y) * WORLD_SCALE;
	transform->destRect.w = srcRect.w * transform->scale.x;
	transform->destRect.h = srcRect.h * transform->scale.y;
}

void TileComponent::Render()
{
	SDL_RenderCopyF(Game::renderer, sprites->GetCurrentTexture(), &srcRect, &transform->destRect);
}