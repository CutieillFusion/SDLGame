#pragma once
#include "ECS.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "TextureManager.h"

class SpriteRendererComponent : public Component
{
public:
	SDL_Texture* texture;

	SpriteRendererComponent() = default;
	SpriteRendererComponent(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = 32;
		srcRect.h = 32;
	}
	void Initialize() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void Update() override
	{
		transform->destRect.x = transform->position.x * WORLD_SCALE;
		transform->destRect.y = transform->position.y * WORLD_SCALE;
		transform->destRect.w = srcRect.w * transform->scale.x;
		transform->destRect.h = srcRect.h * transform->scale.y;
	}

	void Render() override
	{
		SDL_RenderCopyExF(Game::renderer, texture, &srcRect, &transform->destRect, transform->rotationAngle, NULL, transform->flipFlags);
	}

private:
	TransformComponent* transform;
	SDL_Rect srcRect;
};