#include "TileComponent.h"

TileComponent::TileComponent(std::string id)
{
	TileComponent::texture = Game::assets->GetTexture(id);
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = WORLD_SCALE;
	srcRect.h = WORLD_SCALE;
}

void TileComponent::Initialize()
{
	transform = &entity->getComponent<TransformComponent>();
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
	SDL_RenderCopyExF(Game::renderer, texture, &srcRect, &transform->destRect, transform->rotationAngle, NULL, transform->flipFlags);
}
