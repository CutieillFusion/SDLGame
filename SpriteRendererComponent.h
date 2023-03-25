#pragma once
#include "ECS.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "Vector.h"
#include "AssetManager.h"

class SpriteRendererComponent : public Component
{
public:
	SDL_Texture* texture;

	SpriteRendererComponent() = default;
	SpriteRendererComponent(std::string id);

	void Initialize() override;
	void Update() override;
	void Render() override;

private:
	TransformComponent* transform;
	SDL_Rect srcRect;
};