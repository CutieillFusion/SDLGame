#pragma once
#include "ECS.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "CameraComponent.h"
#include "Vector.h"
#include "Game.h"

class SpriteRendererComponent : public Component
{
public:
	SDL_Texture* texture;

	SpriteRendererComponent() = default;
	SpriteRendererComponent(std::string id)
	{
		SpriteRendererComponent::texture = AssetManager::instance->GetTexture(id);
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = WORLD_SCALE;
		srcRect.h = WORLD_SCALE;
	}

	void Initialize() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void Update() override
	{
		transform->destRect.x = (transform->position.x - Game::camera.x) * WORLD_SCALE;
		transform->destRect.y = (transform->position.y - Game::camera.y) * WORLD_SCALE;
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