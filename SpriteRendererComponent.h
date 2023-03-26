#pragma once
#include "ECS.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Vector.h"
#include "AssetManager.h"

class SpriteRendererComponent : public Component
{
public:
	SpriteRendererComponent() = default;

	void Initialize() override;
	void Update() override;
	void Render() override;

private:
	SpriteComponent* sprites;
	TransformComponent* transform;
	SDL_Rect srcRect;
};