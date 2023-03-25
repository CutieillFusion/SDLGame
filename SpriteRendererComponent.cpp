#include "SpriteRendererComponent.h"

SpriteRendererComponent::SpriteRendererComponent(std::string id)
{
	SpriteRendererComponent::texture = Game::assets->GetTexture(id);
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = WORLD_SCALE;
	srcRect.h = WORLD_SCALE;
}

void SpriteRendererComponent::Initialize()
{
	transform = &entity->getComponent<TransformComponent>();
}

void SpriteRendererComponent::Update()
{
	transform->destRect.x = (transform->position.x - Game::camera.x) * WORLD_SCALE;
	transform->destRect.y = (transform->position.y - Game::camera.y) * WORLD_SCALE;
	transform->destRect.w = srcRect.w * transform->scale.x;
	transform->destRect.h = srcRect.h * transform->scale.y;
}

void SpriteRendererComponent::Render()
{
	SDL_RenderCopyExF(Game::renderer, texture, &srcRect, &transform->destRect, transform->rotationAngle, NULL, transform->flipFlags);
}
